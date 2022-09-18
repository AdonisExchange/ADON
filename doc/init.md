Sample init scripts and service configuration for adond
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/adond.service:    systemd service unit configuration
    contrib/init/adond.openrc:     OpenRC compatible SysV style init script
    contrib/init/adond.openrcconf: OpenRC conf.d file
    contrib/init/adond.conf:       Upstart service configuration file
    contrib/init/adond.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "adon" user
and group.  They must be created before attempting to use these scripts.
The macOS configuration assumes adond will be set up for the current user.

Configuration
---------------------------------

At a bare minimum, adond requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, adond will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that adond and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If adond is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running adond without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see contrib/debian/examples/adon.conf.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              /usr/bin/adond
Configuration file:  /etc/adon/adon.conf
Data directory:      /var/lib/adond
PID file:            `/var/run/adond/adond.pid` (OpenRC and Upstart) or `/run/adond/adond.pid` (systemd)
Lock file:           `/var/lock/subsys/adond` (CentOS)

The configuration file, PID directory (if applicable) and data directory
should all be owned by the adon user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
adon user and group.  Access to adon-cli and other adond rpc clients
can then be controlled by group membership.

NOTE: When using the systemd .service file, the creation of the aforementioned
directories and the setting of their permissions is automatically handled by
systemd. Directories are given a permission of 710, giving the adon group
access to files under it _if_ the files themselves give permission to the
adon group to do so (e.g. when `-sysperms` is specified). This does not allow
for the listing of files under the directory.

NOTE: It is not currently possible to override `datadir` in
`/etc/adon/adon.conf` with the current systemd, OpenRC, and Upstart init
files out-of-the-box. This is because the command line options specified in the
init files take precedence over the configurations in
`/etc/adon/adon.conf`. However, some init systems have their own
configuration mechanisms that would allow for overriding the command line
options specified in the init files (e.g. setting `BITCOIND_DATADIR` for
OpenRC).

### macOS

Binary:              `/usr/local/bin/adond`
Configuration file:  `~/Library/Application Support/ADON/adon.conf`
Data directory:      `~/Library/Application Support/ADON`
Lock file:           `~/Library/Application Support/ADON/.lock`

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start adond` and to enable for system startup run
`systemctl enable adond`

NOTE: When installing for systemd in Debian/Ubuntu the .service file needs to be copied to the /lib/systemd/system directory instead.

### OpenRC

Rename adond.openrc to adond and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/adond start` and configure it to run on startup with
`rc-update add adond`

### Upstart (for Debian/Ubuntu based distributions)

Upstart is the default init system for Debian/Ubuntu versions older than 15.04. If you are using version 15.04 or newer and haven't manually configured upstart you should follow the systemd instructions instead.

Drop adond.conf in /etc/init.  Test by running `service adond start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy adond.init to /etc/init.d/adond. Test by running `service adond start`.

Using this script, you can adjust the path and flags to the adond program by
setting the ADOND and FLAGS environment variables in the file
/etc/sysconfig/adond. You can also use the DAEMONOPTS environment variable here.

### macOS

Copy org.adon.adond.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.adon.adond.plist`.

This Launch Agent will cause adond to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run adond as the current user.
You will need to modify org.adon.adond.plist if you intend to use it as a
Launch Daemon with a dedicated adon user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
