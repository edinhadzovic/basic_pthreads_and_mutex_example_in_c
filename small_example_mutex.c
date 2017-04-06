/* A Program that just shows how thread works, and little about mutex */
// This a very baisc introducation to threads and mutex you should read more about it
// Create by Edin Hadzovic - edinhadzovic.com
// to compile it run:  gcc -Wall -o bsp_mutex small_example_mutex.c -lpthread
/* HAVE A NICE DAY ! */
#include <stdio.h>
#include <pthread.h>


char user_input; // storing the user input
int user_move = -1; // a not useful variable
pthread_mutex_t mutex; // we define our mutex here

pthread_t thr_read, thr_right, thr_left, thr_quit; // out threads.

void err_msg(int err_type, const char* string)
{
  printf("%s with a error code of: %d\n", string, err_type);
}

// Our function for our thr_left thread.
void* user_go_left(void *args){
    if(user_input == 'a') {
      printf("%s\n", "i guess you enter a");
      user_move = 0;
    }
  // user_move = 0;
  pthread_exit((void*)0); // exit the thread and we can add some value to it if
                          // we want to get it. so exp: pthread_exit(100). this
                          // value we will get if we call pthread_join(thrad, status)
                          // it will be stored in status.
}

// Our function for our thr_right thread EVERYTHING SAME
void* user_go_right(void* args){
  if(user_input == 'd'){
    printf("%s\n", "i guess you enter d");
    user_move = 1;
  }
  pthread_exit((void*)0);
}
//Our function for our thr_quit thread
void* user_quits(void *args){
  // user_move = 2;
  if(user_input == 'q')
    printf("%s\n", "i guess you enter q");
    user_move = 2;
  pthread_exit((void*)0);
}

// Our function for our "main" thread because you can see we create other threads
// from here.
void* read_user_input(void *args){
  int err;
  while((user_input = getchar())){
    pthread_mutex_lock(&mutex); // lock the mutex
    if (user_input == 'q') {
      err = pthread_create(&thr_quit, NULL, user_quits, NULL); // create the thread
        if (err) err_msg(err, "Couldn't not create Thread");
      err = pthread_join(thr_quit, NULL); //wait for thread to finish
        if (err) err_msg(err, "Couldn't not join Thread");
      pthread_exit((void*)0);
    } else if(user_input == 'a') {
      err = pthread_create(&thr_left, NULL, user_go_left, NULL);
        if (err) err_msg(err, "Couldn't not create Thread");
      err = pthread_join(thr_left, NULL);
        if (err) err_msg(err, "Couldn't not join Thread");
    } else if(user_input == 'd') {
      err = pthread_create(&thr_right, NULL, user_go_right, NULL);
        if (err) err_msg(err, "Couldn't not create Thread");
      err = pthread_join(thr_right, NULL);
        if (err) err_msg(err, "Couldn't not join Thread");
    }
    pthread_mutex_unlock(&mutex); // unlock the mutex

  }
  pthread_exit((void*)0);
}

int main(int argc, char const *argv[]) {

  int quit;
  int err;
  pthread_mutex_init(&mutex, NULL); // init. our mutex always destory it at the end;

  err = pthread_create(&thr_read, NULL, read_user_input, NULL); // create MAIN Thread
  if(err) if (err) err_msg(err, "Couldn't not create Thread");  // if couldn't make it.

  while(!quit){
    if (user_move == 2) {
      quit = 1;
    }
  }

  printf("%s\n", "bye");
  pthread_join(thr_read, NULL); // cleaning our main thread
  pthread_mutex_destroy(&mutex); // cleaning our mutex because we use pthread_mutex_init()
                                 // so we need to destory it before we exit our program.
  return 0;
}
