#!/bin/bash

WATCH_EXT="/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/.extended_list"
PROJECT_DIR="/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting"
CPP_INSERT="build/log_parser"

touch "${WATCH_EXT}"


while read -r FILE EVENT
do
    echo "INFO: ${LOG_TIME} Programme Info: Watch File: ${EXT_LOG}: ${EVENT}"
    EXT_LOG_FILE=$(cat ${FILE})
    ${PROJECT_DIR}/${CPP_INSERT} insert extended ${EXT_LOG_FILE}
    echo "INFO: Log file ${EXT_LOG_FILE} Inserted into Database"    
done < <(inotifywait -q -e modify -m "${WATCH_EXT}")
