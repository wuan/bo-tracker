#
# Regular cron jobs for the blitzortung-tracker package
#

SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# m h dom mon dow user  command
*/5 * * * *	root	ps >/dev/null -C blitzortung-tracker || /etc/init.d/blitzortung-tracker start 
