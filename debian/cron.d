#
# Regular cron jobs for the blitzortung-tracker package
#

SHELL=/bin/sh
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# m h dom mon dow user  command
* * * * *	root	ps >/dev/null -C bo-tracker ||(service blitzortung-tracker start)
