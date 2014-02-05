-module(client).
-import(server, [start/0, katt/0]).

main(_) ->
  Server = server:start(),
  Server ! {self(), broadcast, "Anyone here?"}, read_message(),
  Server ! {self(), login, "Guido Fawkee"}, read_message(),
  Server ! {self(), get_users}, read_message(),
  Server ! {self(), broadcast, "Hello! I'M HERE!"}, read_message(),
  Server ! {self(), logout}, read_message(),
  Server ! {self(), get_users}, read_message().
  
read_message() ->
  receive
    {msg, Message} ->
      io:format("got msg: ~s~n", [Message]);
      
    {users, Users} ->
      io:format("got user list: ~p~n", [Users])
      
  after 1000 ->
    io:format("nothing :'(~n")
  end.
  