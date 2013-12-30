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

static long long createJobId(redisContext *context) {
   redisReply *reply = redisCommand(context, "INCR jobs_last_id");
   if (reply->type == REDIS_REPLY_INTEGER) {
      printf("Got new id: %lld\n", reply->integer);
      return reply->integer;
   }   
   
   printf("Failed.\n");
   exit(1);
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

      char job_id[64] = {0};
      long long job_num_id = createJobId(context);
      sprintf(job_id, "job:%lld", job_num_id);
      
      redisReply *reply = redisCommand(context, "HSET %s name %s", job_id, job_name);
      
      reply = redisCommand(context, "RPUSH jobs %lld", job_num_id);
   }
   
   redisFree(context);
   
   return 0;
}
