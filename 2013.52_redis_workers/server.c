#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>

typedef struct {
   char *name;
   char *city;
   char *body;
} job_t;

static void read_input(char *output, char *data, size_t n) {
   printf("%s", output);

   char *buf = 0;
   size_t linecap = 0;
   if (!getline(&buf, &linecap, stdin)) {
      exit(0);
   }

   strncpy(data, buf, n);
   size_t len = strlen(data);
   if (data[len - 1] == '\n') {
      data[len - 1] = '\0';
   }
}

static long long createJobId(redisContext *context) {
   redisReply *reply = redisCommand(context, "INCR jobs_last_id");
   assert(reply->type == REDIS_REPLY_INTEGER);
   
   return reply->integer;
}

static void setKeyField(redisContext *context, char *job_id, char *field, char *value) {
   redisReply *reply = redisCommand(context, "HSET %s %s %s", job_id, field, value);
   assert(reply->type != REDIS_REPLY_ERROR);
}

static long long createJob(redisContext *context, job_t *job) {
   long long job_num_id = createJobId(context);

   char job_id[64] = {0};
   snprintf(job_id, sizeof job_id - 1, "job:%lld", job_num_id);
   
   setKeyField(context, job_id, "name", job->name);
   setKeyField(context, job_id, "city", job->city);
   setKeyField(context, job_id, "body", job->body);
   redisReply *reply = redisCommand(context, "RPUSH jobs %lld", job_num_id);
     
   return job_num_id;
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
      char job_city[256] = {0};
      char job_body[256] = {0};
      
      read_input("name: ", job_name, sizeof job_name - 1);
      read_input("city: ", job_city, sizeof job_city - 1);
      read_input("body: ", job_body, sizeof job_body - 1);

      job_t job;
      job.name = job_name;
      job.city = job_city;
      job.body = job_body;
      
      long long job_id = createJob(context, &job);
      printf("Created job %lld\n", job_id);
   }
   
   redisFree(context);
   
   return 0;
}
