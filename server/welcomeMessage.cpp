#include "../libs.hpp"

/*

Connection Registration
Immediately upon establishing a connection the client must attempt registration,
 without waiting for any banner message from the server.

Until registration is complete, only a limited subset of commands SHOULD be accepted by the server.
This is because it makes sense to require a registered (fully connected) client connection before allowing
commands such as JOIN, PRIVMSG and others.

The recommended order of commands during registration is as follows:

CAP LS 302
PASS
NICK and USER
Capability Negotiation
SASL (if negotiated)
CAP END

The commands specified in steps 1-3 should be sent on connection.
If the server supports capability negotiation then registration will be suspended and the client can negotiate
client capabilities (steps 4-6). If the server does not support capability negotiation then registration will
continue immediately without steps 4-6.

If the server supports capability negotiation, the CAP command suspends the registration process and immediately
starts the capability negotiation process. CAP LS 302 means that the client supports version 302 of client capability
negotiation. The registration process is resumed when the client sends CAP END to the server.

The PASS command is not required for the connection to be registered, but if included it MUST precede the latter of
the NICK and USER commands.

The NICK and USER commands are used to set the user’s nickname, username and “real name”. Unless the registration
is suspended by a CAP negotiation, these commands will end the registration process.

The client should request advertised capabilities it wishes to enable here.

If the client supports SASL authentication and wishes to authenticate with the server, it should attempt this after a
successful CAP ACK of the sasl capability is received and while registration is suspended.

If the server support capability negotiation, CAP END will end the negotiation period and resume the registration.

If the server is waiting to complete a lookup of client information (such as hostname or ident for a username),
there may be an arbitrary wait at some point during registration. Servers SHOULD set a reasonable timeout for these lookups.

Additionally, some servers also send a PING and require a matching PONG from the client before continuing.
This exchange may happen immediately on connection and at any time during connection registration, so clients MUST respond correctly to it.

Upon successful completion of the registration process, the server MUST send, in this order:

RPL_WELCOME (001),
RPL_YOURHOST (002),
RPL_CREATED (003),
RPL_MYINFO (004),
at least one RPL_ISUPPORT (005) numeric to the client.
The server MAY then send other numerics and messages.
The server SHOULD then respond as though the client sent the LUSERS command and return the appropriate numerics.
The server MUST then respond as though the client sent it the MOTD command, i.e.
it must send either the successful Message of the Day numerics or the ERR_NOMOTD (422) numeric.
If the user has client modes set on them automatically upon joining the network, the server SHOULD send the
client the RPL_UMODEIS (221) reply or a MODE message with the client as target, preferably the former.
The first parameter of the RPL_WELCOME (001) message is the nickname assigned by the network to the client.
Since it may differ from the nickname the client requested with the NICK command
(due to, e.g. length limits or policy restrictions on nicknames), the client SHOULD use this parameter to
determine its actual nickname at the time of connection. Subsequent nickname changes, client-initiated or not,
will be communicated by the server sending a NICK message.

*/



/*

 * ---------------------------------------------------------------------------------------------------- *

LUSERS message

  Command: LUSERS
  Parameters: None

Returns statistics about local and global users, as numeric replies.

Servers MUST reply with RPL_LUSERCLIENT and RPL_LUSERME, and SHOULD also include all those defined below.

Clients SHOULD NOT try to parse the free-form text in the trailing parameter, and rely on specific parameters instead.

RPL_LUSERCLIENT (251)
RPL_LUSEROP (252)
RPL_LUSERUNKNOWN (253)
RPL_LUSERCHANNELS (254)
RPL_LUSERME (255)
RPL_LOCALUSERS (265)
RPL_GLOBALUSERS (266)

 * ---------------------------------------------------------------------------------------------------- *

RPL_LUSERCLIENT (251)

  "<client> :There are <u> users and <i> invisible on <s> servers"

Sent as a reply to the LUSERS command. <u>, <i>, and <s> are non-negative integers,
and represent the number of total users, invisible users, and other servers connected to this server.

 * ---------------------------------------------------------------------------------------------------- *

RPL_LUSEROP (252)

  "<client> <ops> :operator(s) online"

Sent as a reply to the LUSERS command. <ops> is a positive integer and represents the number of
IRC operators connected to this server. The text used in the last param of this message may vary.

 * ---------------------------------------------------------------------------------------------------- *

RPL_LUSERUNKNOWN (253)

  "<client> <connections> :unknown connection(s)"

Sent as a reply to the LUSERS command. <connections> is a positive integer and represents the number
of connections to this server that are currently in an unknown state. The text used in the last
param of this message may vary.

 * ---------------------------------------------------------------------------------------------------- *

RPL_LUSERCHANNELS (254)

  "<client> <channels> :channels formed"

Sent as a reply to the LUSERS command. <channels> is a positive integer and represents the number of
channels that currently exist on this server. The text used in the last param of this message may vary.

 * ---------------------------------------------------------------------------------------------------- *

RPL_LUSERME (255)

  "<client> :I have <c> clients and <s> servers"

Sent as a reply to the LUSERS command. <c> and <s> are non-negative integers and represent the number of
clients and other servers connected to this server, respectively.

 * ---------------------------------------------------------------------------------------------------- *

RPL_LOCALUSERS (265)

  "<client> [<u> <m>] :Current local users <u>, max <m>"

Sent as a reply to the LUSERS command. <u> and <m> are non-negative integers and represent
the number of clients currently and the maximum number of clients that have been connected
directly to this server at one time, respectively.

The two optional parameters SHOULD be supplied to allow clients to better extract these numbers.

 * ---------------------------------------------------------------------------------------------------- *

RPL_GLOBALUSERS (266)

  "<client> [<u> <m>] :Current global users <u>, max <m>"

Sent as a reply to the LUSERS command. <u> and <m> are non-negative integers.
<u> represents the number of clients currently connected to this server,
globally (directly and through other server links).
<m> represents the maximum number of clients that have been
connected to this server at one time, globally.

The two optional parameters SHOULD be supplied to allow clients to better extract these numbers.

 * ---------------------------------------------------------------------------------------------------- *

*/


/*
 * ---------------------------------------------------------------------------------------------------- *

RPL_WELCOME (001)

  "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"

The first message sent after client registration, this message introduces the client to the network.
The text used in the last param of this message varies wildly.
Servers that implement spoofed hostmasks in any capacity SHOULD NOT include the extended (complete)
hostmask in the last parameter of this reply, either for all clients or for those whose hostnames
have been spoofed. This is because some clients try to extract the hostname from this final
parameter of this message and resolve this hostname, in order to discover their ‘local IP address’.
Clients MUST NOT try to extract the hostname from the final parameter of this message and then
attempt to resolve this hostname. This method of operation WILL BREAK and will cause issues when
the server returns a spoofed hostname.

 * ---------------------------------------------------------------------------------------------------- *

RPL_YOURHOST (002)

  "<client> :Your host is <servername>, running version <version>"

Part of the post-registration greeting, this numeric returns the name and software/version of the
server the client is currently connected to. The text used in the last param of this message varies wildly.

 * ---------------------------------------------------------------------------------------------------- *

RPL_CREATED (003)

  "<client> :This server was created <datetime>"

Part of the post-registration greeting, this numeric returns a human-readable date/time that the server
 was started or created. The text used in the last param of this message varies wildly.

 * ---------------------------------------------------------------------------------------------------- *

RPL_MYINFO (004)

  "<client> <servername> <version> <available user modes> <available channel modes> [<channel modes with a parameter>]"

Part of the post-registration greeting. Clients SHOULD discover available features using RPL_ISUPPORT
tokens rather than the mode letters listed in this reply.

 * ---------------------------------------------------------------------------------------------------- *

RPL_ISUPPORT (005)

  "<client> <1-13 tokens> :are supported by this server"

The ABNF representation for an RPL_ISUPPORT token is:

  token      =  *1"-" parameter / parameter *1( "=" value )
  parameter  =  1*20 letter
  value      =  * letpun
  letter     =  ALPHA / DIGIT
  punct      =  %d33-47 / %d58-64 / %d91-96 / %d123-126
  letpun     =  letter / punct

As the maximum number of message parameters to any reply is 15, the maximum number of RPL_ISUPPORT tokens
that can be advertised is 13. To counter this, a server MAY issue multiple RPL_ISUPPORT numerics. A server
MUST issue at least one RPL_ISUPPORT numeric after client registration has completed. It MUST be issued
before further commands from the client are processed.

When clients send a VERSION command to an external server (i.e. not the one they’re currently connected to),
they receive the appropriate information from that server. That external server’s ISUPPORT tokens are sent
to the client using the 105 (RPL_REMOTEISUPPORT) numeric instead of 005, to ensure that clients don’t process
and start using these tokens sent by an external server. The format of the 105 message is exactly the same as
RPL_ISUPPORT – the numeric itself is the only difference.

A token is of the form PARAMETER, PARAMETER=VALUE or -PARAMETER. Servers MUST send the parameter as upper-case text.

Tokens of the form PARAMETER or PARAMETER=VALUE are used to advertise features or information to clients.
A parameter MAY have a default value and value MAY be empty when sent by servers. Unless otherwise stated,
when a parameter contains a value, the value MUST be treated as being case sensitive.
The value MAY contain multiple fields, if this is the case the fields SHOULD be delimited with a comma character (",", 0x2C).
The value MAY contain escape sequences: \x20 for the space character (" ", 0x20), \x5C for the backslash character ("\", 0x5C)
and \x3D for the equal character ("=", 0x3D).

If the value of a parameter changes, the server SHOULD re-advertise the parameter with the new value in an RPL_ISUPPORT reply.
An example of this is a client becoming an IRC operator and their CHANLIMIT changing.

Tokens of the form -PARAMETER are used to negate a previously specified parameter. If the client receives a token like this,
the client MUST consider that parameter to be removed and revert to the behaviour that would occur if the parameter was not specified.
The client MUST act as though the paramater is no longer advertised to it.
These tokens are intended to allow servers to change their features without disconnecting clients.
Tokens of this form MUST NOT contain a value field.

The server MAY negate parameters which have not been previously advertised; in this case, the client MUST ignore the token.

A single RPL_ISUPPORT reply MUST NOT contain the same parameter multiple times nor advertise and negate the same parameter.
However, the server is free to advertise or negate the same parameter in separate replies.

See the Feature Advertisement section for more details on this numeric.
A list of parameters is available in the RPL_ISUPPORT Parameters section.

 * ---------------------------------------------------------------------------------------------------- *
 */