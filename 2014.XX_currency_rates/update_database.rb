#!/usr/bin/env ruby

# Redis structure
# ---------------
#
# usd_rates:2013-04-23
#   aed
#   afn
#   all
#
# date is non-extended iso8601 (YYYY-MM-DD).
#
# etc.

require 'json'
require 'redis'
require 'date'
require 'net/http'

$app_id = 'f28059fea8524dcfa5c78853916f8258'

# Downloads the latest rates from http://openexchangerates.org, returns them 
# in JSON format.
def retrieve_rates(date)
  path = "http://openexchangerates.org/api/historical/#{date.to_s}.json?app_id=#{$app_id}"
  response = Net::HTTP.get_response(URI.parse path)
  # TODO: error handling
  response.body
end

# Writes rates for usd to redis db
def upload_usd_rates(date, rates)
  redis = Redis.new
  key = "usd_rates:#{date.to_s}"

  rates.each do |currency, rate|
    redis.hset(key, currency.downcase, rate)
  end
end

# Downloads and updates the Redis db with rates for the date
def load_rates(date)
  data = retrieve_rates date
  rate_snapshot = JSON.parse data
  exit 1 unless rate_snapshot['base'].downcase == 'usd'  # TODO: better error handling

  rate_date = DateTime.strptime(rate_snapshot['timestamp'].to_s, '%s').to_date

  upload_usd_rates(rate_date, rate_snapshot['rates'])
  
  puts "Updated rates for #{rate_date.to_s}"
end


load_rates Date.today
