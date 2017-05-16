#!/bin/sh

cd /home/tadams2/analytics/

LOG_DIR=/home/tadams2/analytics/tweetindex/log
LOG_FILE=$LOG_DIR/`date +%Y%m%d_%H%M%S`.log
export JAVA_HOME=/usr/java/default
export PATH=/opt/hadoop/bin:$PATH
mkdir -p $LOG_DIR

PIG_SCRIPT=revIndex.pig
OUTPUT=tweetindex/`date +%Y/%m/%d/%H`
INPUT=/in/tweets/`date -u -d '1 hour ago' +%Y/%m/%d/%H`


RUN_CMD="/opt/pig/bin/pig -p input=$INPUT -p output=$OUTPUT -f $PIG_SCRIPT"
echo $RUN_CMD
$RUN_CMD &> $LOG_FILE