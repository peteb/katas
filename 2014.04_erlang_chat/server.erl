-module(server).
-export([start/0]).

start() ->
  spawn(fun() -> read_message([]) end).
  
broadcast([], _) -> ok;
broadcast([{_, Pid}|T], Message) ->
  Pid ! {msg, Message},
  broadcast(T, Message).
  
read_message(Users) ->
  receive
    {From, login, Name} ->
      From ! {msg, "Welcome " ++ Name ++ "!"},
      monitor(process, From),
      read_message(Users ++ [{Name, From}]);
      
    {From, get_users} ->
      From ! {users, lists:map(fun({Username, _}) -> Username end, Users)},
      read_message(Users);
      
    {From, logout} ->
      case lists:keyfind(From, 2, Users) of       
        {Username, _} ->
          From ! {msg, "Good-bye, " ++ Username},
          UsersLeft = lists:filter(fun({_, Pid}) -> Pid /= From end, Users),
          read_message(UsersLeft);
          
        _ ->
          read_message(Users)
      end;
      
    {'DOWN', _, _, DeadPid, _} ->
      UsersLeft = lists:filter(fun({_, Pid}) -> Pid /= DeadPid end, Users),
      read_message(UsersLeft);
      
    {_, broadcast, Message} ->
      broadcast(Users, Message),
      read_message(Users)
  end.