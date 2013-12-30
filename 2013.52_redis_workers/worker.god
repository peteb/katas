God.watch do |w|
  w.name = "mail generator"
  w.start = "ruby `pwd`/worker.rb"
  w.log = '/var/log/mailgenerator.log'
  w.keepalive  
end
