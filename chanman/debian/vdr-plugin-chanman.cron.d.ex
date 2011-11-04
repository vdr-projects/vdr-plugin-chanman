#
# Regular cron jobs for the vdr-plugin-chanman package
#
0 4	* * *	root	[ -x /usr/bin/vdr-plugin-chanman_maintenance ] && /usr/bin/vdr-plugin-chanman_maintenance
