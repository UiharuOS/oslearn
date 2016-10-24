void function(int a, int b) {
  int d = 0;
}

int main(void) {
  int a = 0;  
  function(1, 2); // pthread_create -> posix thread
  a++;
}
