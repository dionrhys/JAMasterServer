// JAMS RCON Command System
// <3 Q3

/**
 * stop
 * Gracefully shutdown the server remotely
 *
 * Usage:
 *   stop
 *
 * WARNING: You won't get it back up with rcon again :P
 */



/**
 * restart
 * Gracefully restart the server remotely
 *
 * Usage:
 *   restart
 */



/**
 * kick
 * Kick a server from our server list
 *
 * Usage:
 *   kick <server ip> - Kick all registered servers using this IP
 *                      (Note: Servers can exist on different ports)
 *
 *   kick <server ip:port> - Kick the registered server using this IP/Port
 *                           (Guaranteed to be only one server)
 *
 *   kick all - Kick ALL the currently registered servers off the list!
 *
 * This command supports wildcard IP address matches (192.168.*.*)
 */



/**
 * ban
 * Ban a server from our server list
 *
 * Usage:
 *   ban <server ip> - Ban any server from this IP from registering
 *                     (Note: Servers can exist on different ports)
 *
 *   ban <server ip:port> - Ban the server at this IP/Port from registering
 *                          (Guaranteed to be only one server)
 *
 * This command supports wildcard IP address matches (192.168.*.*)
 */



/**
 * status
 * Get a brief status of the server
 *
 * Usage:
 *   status
 *
 * Example response:
 *   Servers online: XXX
 *
 */



/**
 * listservers
 * List pages of registered servers
 *
 * Usage:
 *   list [page number] - Responds with a single page of registered servers
 *                        If no page number is given, page 1 is shown
 *                        (Pages contain <listserversPageCount> servers each)
 *
 *   list * - Show every single registered server
 *            (WARNING: Will generate a lot of UDP traffic!)
 *
 * Example response:
 *   #    Address         Port  Timeout
 *   ---- --------------- ----- -------
 *      0 4.4.4.4         8     7s
 *     10 9.87.6.543      1056  11s
 *     20 54.321.978.6    135   254s
 *   ...
 *    300 11.22.33.44     5000  36s
 *   4000 123.123.123.123 29070 104s
 *   --- Page 1/1 ---------------------
 */



/**
 * showgraph
 * Print an ASCII art graph :') of the registered server count
 * over a day in 30 minute intervals
 *
 * Usage:
 *   showgraph
 *
 * Example response:
 *   600 |
 *   570 |
 *   540 |
 *   510 |
 *   480 |                     _________
 *   450 |                _____|       |
 *   420 |                |            |_
 *   390 |              __|             |___
 *   360 |             _|                  |
 *   330 |             |                   |__
 *   300 |             |                     |
 *   270 |             |                     |
 *   240 |             |                     |  _
 *   210 |            _|                     |__|_
 *   180 |            |                          |
 *   150 |            |                          |
 *   120 |       ___ _|                          |__
 *    90 |       | |_|                             |__
 *    60 |     __|                                   |__
 *    30 |_____|                                       |__
 *     0 |________________________________________________
 *        ^^^^^|^^^^^|^^^^^|^^^^^|^^^^^|^^^^^|^^^^^|^^^^^|
 *            18    21    00    03    06    09    12    15
 */