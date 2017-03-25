/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Descripción: Este archivo contiene varias definiciones de las funciones
 * utilizadas a lo largo del thread-list: análisis de argumentos, inicialización
 * de estructuras, manejo de hilos, exploración de directorios, asignación y
 * escritura de salida
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
 * Marzo, 2017
 *
*/

#include "tls.h"

void help(void){
	execlp("pager", "pager", "help");
}

void usage(void){
	printf("uso: tls [-h] | [-n i] [-d directorio] [salida]\n");
	printf("ejecute tls -h para mayor información\n");
	exit(0);
}

void init_inputargs(Inargs *in){
	in->concurrency = 1;
	getcwd(in->directory, PATH_MAX);
	strcpy(in->out, "stdout");
}

void parseArgs(Inargs *in, int argc, char *argv[]){
	int ch, index;
	bool h, nd;

	h = false;
	nd = false;

	while( (ch = getopt(argc, argv, "hn:d:")) != -1)
		switch(ch){
			case 'h':
				h = true;
				break;
			case 'n':
				if (!isdigit(optarg[0])) usage();
				in->concurrency = atoi(optarg);
				nd = true;
				break;
			case 'd':
				strcpy(in->directory, optarg);
				nd = true;
				break;
			case '?':
				usage();
			default:
				usage();
		}

	if(h && nd) usage();
	else if (h) help();

	if(optind < argc) strcpy(in->out, argv[optind]);
}

void init_threadstruct(Threadstruct *t, int thnum, List *dirlist,
												List* infolist, List *idlelist){
  t->id = 0;
	t->thnum = thnum;
	t->has_dirallocated = 0;
  t->dirlist = dirlist;
  t->infolist = infolist;
	t->idlelist = idlelist;
}

void init_information(Information *i, long id, char *dir, int fcount,
												int bcount){
	i->id = id;
	strcpy(i->path, dir);
	i->fcount = fcount;
	i->bcount = bcount;
}

int onwork(int thnum, List* idlelist){
	return (thnum != idlelist->size);
}

void *threadmgmt(void *structure){
  Threadstruct *t;
	Node *n;
	int i;

	t = (Threadstruct *) structure;
	t->id = pthread_self();

	while(!empty(t->dirlist) || onwork(t->thnum, t->idlelist)){
		if(t->has_dirallocated){
      // Thread has a dir allocated by master thread
      explore(t);
			t->has_dirallocated = 0;

			n = (Node *) malloc(sizeof(Node));
			init_node(n, t);

			pthread_mutex_lock(&idlemutex);
			add(t->idlelist, n);
			pthread_mutex_unlock(&idlemutex);
    }
  }
	pthread_exit(NULL);
}

void createThreads(Threadstruct *master, pthread_t *threads){
  int rc, i;
	Threadstruct *t;
	Node *n;

  for (i = 0; i < master->thnum; i++) {
    t = (Threadstruct *) malloc(sizeof(Threadstruct));
    init_threadstruct(t, master->thnum, master->dirlist, master->infolist,
											master->idlelist);

		n = (Node *) (malloc(sizeof(Node)));
		init_node(n, t);

		add(master->idlelist, n);

    rc = pthread_create(&threads[i], NULL, threadmgmt, t);

		if (rc){
      printf("Error al crear hilo. Código de error:%d\n", rc);
      exit(-1);
    }
  }
}

void explore(Threadstruct *t){
	DIR *dp;
	struct dirent *ep;
	struct stat statbuffer;
	char subdir[PATH_MAX];
	char file[PATH_MAX];
	Node *n;
	Directory *d;
	Information *i;
	int filescount, bytescount;

	filescount = 0;
	bytescount = 0;

	printf("Hilo %ld explorando %s\n", t->id, t->directory);

	dp = opendir(t->directory);

	if(!dp){
		perror("No fue posible abrir el directorio\n");
		return;
	}

	while((ep = readdir(dp)) != NULL){
		// Ignore hiden directories
		if(ep->d_name[0] != '.'){

			strcpy(file, t->directory);
			strcat(file, "/");
			strcat(file, ep->d_name);

			if(lstat(file, &statbuffer) == -1){
				perror("No se pudo obtener la información del archivo\n");
				return;
			}

			if(S_ISDIR(statbuffer.st_mode)){
				n = (Node *) malloc(sizeof(Node));
				d = (Directory *) malloc(sizeof(Directory));

				strcpy(subdir, t->directory);
				strcat(subdir, "/");
				strcat(subdir, ep->d_name);

				strcpy(d->dir, subdir);
				init_node(n, d);

				pthread_mutex_unlock(&dirmutex);
				add(t->dirlist, n);
				pthread_mutex_unlock(&dirmutex);
				continue;
			}

			else{
				filescount++;
				bytescount += statbuffer.st_size;
				continue;
			}
		}
	}
	closedir(dp);

	n = (Node *) malloc(sizeof(Node));
	i = (Information *) malloc(sizeof(Information));
	init_information(i, t->id, t->directory, filescount, bytescount);
	init_node(n, i);

	pthread_mutex_lock(&infomutex);
	add(t->infolist, n);
	pthread_mutex_unlock(&infomutex);
}

void allocateDir(Threadstruct *master){
	Node *c, *d;
	Threadstruct *t;
	char dir[PATH_MAX];

	// The master thread will allocate directories while the list isnt empty
	// and threads are working
	while(!empty(master->dirlist) || onwork(master->thnum, master->idlelist)){
		if(master->idlelist->size != 0 && master->dirlist->size != 0){

			// Critical region: Master thread takes an idle thread
			pthread_mutex_lock(&idlemutex);
			c = get(master->idlelist);
			t = (Threadstruct *) c->content;
			pthread_mutex_unlock(&idlemutex);

			// Critical region: Master thread takes a directory to allocate
			pthread_mutex_lock(&dirmutex);
			d = get(master->dirlist);
			t->has_dirallocated = 1;
			strcpy(t->directory, d->content);
			pthread_mutex_unlock(&dirmutex);

		}
	}
}

void writeInformation(Threadstruct *master, char *out){
	FILE *f;
	Node *first, *next;
	Information *info;
	bool file_specified;

	file_specified = false;

	if(strcmp(out,"stdout")){
		f = fopen(out, "w");
		file_specified = true;
	}

	if(master->infolist->size){
		first = master->infolist->first;
		info = (Information *) first->content;

		if (file_specified) fprintf(f, "%ld %s %d %d\n", info->id, info->path,
																	info->fcount, info->bcount);

		else fprintf(stdout, "%ld %s %d %d\n", info->id, info->path,
									info->fcount, info->bcount);

		next = first->next;

		while(next){
			info = (Information *) next->content;

			if (file_specified) fprintf(f, "%ld %s %d %d\n", info->id, info->path,
																		info->fcount, info->bcount);

			else fprintf(stdout, "%ld %s %d %d\n", info->id, info->path,
										info->fcount, info->bcount);
			
			next = next->next;
		}
	}
}
