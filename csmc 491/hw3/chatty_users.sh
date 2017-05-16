#!/bin/sh

cd /home/tadams2/analytics/

export JAVA_HOME=/usr/java/default
export PATH=/opt/hadoop/bin:$PATH
LOG_DIR=/home/tadams2/analytics/chatty/log
LOG_FILE=$LOG_DIR/`date +%Y%m%d_%H%M%S`.log

mkdir -p $LOG_DIR

PIG_SCRIPT=chatty.pig
OUTPUT=chatty/`date +%Y/%m/%d/%H`
INPUT=/in/tweets/`date -u -d '1 hour ago' +%Y/%m/%d/%H`
#INPUT=/in/tweets/2016/03/24/04/tweets-1458810496.avro

RUN_CMD="/opt/pig/bin/pig -p input=$INPUT -p output=$OUTPUT -f $PIG_SCRIPT"
echo $RUN_CMD
$RUN_CMD &> $LOG_FILE