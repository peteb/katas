#!/usr/bin/env ruby

require 'redis'

# Redis keys
# ----------
# jobs = queue of ids
# job:{id} = hash
#   name
#   city
#   body
#   path - path to generated document, set by worker
#
# Published messages
# ------------------
# job_finished - id of job generated

def collectJob(redis, job_id) 
  fields = {}
  # TODO: prettify
  fields['name'] = redis.hget("job:#{job_id}", 'name')
  fields['city'] = redis.hget("job:#{job_id}", 'city')
  fields['body'] = redis.hget("job:#{job_id}", 'body')
  fields
end

# Returns fs path to generated file
def generateMail(job)
  'todo'
end


redis = Redis.new

loop do
  job_id = redis.blpop('jobs')[1]
  
  if redis.exists("job:#{job_id}")
    job = collectJob(redis, job_id)
    p job
    pdf_path = generateMail(job)
    redis.hset("job:#{job_id}", 'path', pdf_path)
    redis.publish('job_finished', job_id)
  end
end
