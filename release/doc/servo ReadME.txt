Allows control of multiple characters on the same computer, across a 
LAN, and possibly across the internet.



The plugin has two distinct modes of operation. 


In server mode, it has the ability to send commands to any clients 
connected to it, either targeted at a single client or all of them at 
once. A server cannot control clients that have not explicitly 
connected to it.


In client mode, it has the ability to connect to a server and receive 
and execute commands targeted at either themselves or everyone. A 
client cannot be controlled unless it has connected to the server 
it wants to control it.


All communications are plain-text, which means if you are working 
across a network, you are subject to monitoring. I can't imagine 
wanting to send sensitive data with this plugin, but you've been
warned anyway.



To load the plugin, type:
/load Servo



Commands:

/servo on

Turns Servo on in Server mode. An optional extra parameter can be 
passed in to specify a valid bind hostmask. If it is not given, the 
server will be bound to 127.0.0.1 (localhost) and will not accept 
conections across any networks.


/servo off

Puts Servo into a neutral state where it can neither send nor 
receive commands.


/servo sync

Puts Servo into the Client mode. An optional extra parameter can be 
passed in to specify a server to connect to (valid IP address or DNS 
name is fine).  If it is not given, the client will connect to 
localhost.


/servo send ...

Sends a command to all connected clients. You do not need to quote 
the command or escape anything, though if you want to have some 
special tags parsed before the command is sent to the clients, see
the list of tags in the next section.

/servo sendto Name ...

As above, except it sends a command that will only be processed by 
the specified client.



Tags:
Most FFXI Tags such as <me> or <t> will be processed by the clients.
I have made a few tags that will work as stand-ins so they can be
processed by the server instead.

[me] - Replaced with the name of the character running the server.

[t] - Replaced with the target id of the server player's target.

[p0]-[p5] - Replaced with the name of the party member in the order 
of the server player's party list.