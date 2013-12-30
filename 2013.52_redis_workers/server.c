#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

static void read_line(char *out, size_t n) {
   char *buf;
   size_t linecap = 0;
   if (!getline(&buf, &linecap, stdin)) {
      exit(0);
   }

   strncpy(out, buf, n);
}

int main() {
   printf("Welcome to Mail Generator Producer.\n");
	
   redisContext *context = redisConnect("127.0.0.1", 6379);
   if (!context || context->err) {
      printf("Error: %s\n", context->errstr);
      exit(1);
   }
   
   while (!feof(stdin)) {
      char job_name[256] = {0};
      
      printf("name: ");
      read_line(job_name, sizeof job_name - 1);

      redisReply *reply = redisCommand(context, "INCR jobs_last_id");
      if (reply->type == REDIS_REPLY_INTEGER) {
         printf("Got new id: %lld\n", reply->integer);
      }
   }
   
   return 0;
}
