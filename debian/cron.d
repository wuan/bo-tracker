#
# Regular cron jobs for the blitzortung-tracker package
#

SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# m h dom mon dow user  command
*/15 * * * *	root	[ -d /var/www ] && blitzortung-info /var/www/
