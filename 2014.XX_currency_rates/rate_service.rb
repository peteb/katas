#!/usr/bin/env ruby

require 'sinatra'
require 'redis'

# Retrieves the conversion rate between currency_a and currency_b
# at the given date.
def retrieve_rate(currency_a, currency_b, date)
  redis = Redis.new
  key = "usd_rates:#{date.to_s}"
  halt(404, 'No rates for given date') unless redis.exists key
  
  rate_a = redis.hget(key, currency_a.downcase)
  rate_b = redis.hget(key, currency_b.downcase)

  rate_b.to_f / rate_a.to_f  # TODO: precision
end

get '/rates' do 
  from = params['from'] || halt('Missing from parameter')
  to = params['to'] || halt('Missing to parameter')
  date = params['date'] || halt('Missing date parameter')
  
  retrieve_rate(from, to, date).to_s
end

