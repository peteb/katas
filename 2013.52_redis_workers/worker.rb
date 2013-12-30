#!/usr/bin/env ruby

require 'redis'

# Redis keys
# ----------
# jobs = queue of ids
# job:{id} = hash
#   name
#   city
#   body
#

def collectJob(redis, job_id) 
  fields = {}
  # TODO: prettify
  fields['name'] = redis.hget("job:#{job_id}", 'name')
  fields['city'] = redis.hget("job:#{job_id}", 'city')
  fields['body'] = redis.hget("job:#{job_id}", 'body')
  fields
end

redis = Redis.new

loop do
  job_id = redis.blpop('jobs')[1]
  
  if redis.exists("job:#{job_id}")
    job = collectJob(redis, job_id)
  end
end
