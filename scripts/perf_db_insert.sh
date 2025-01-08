#!/bin/bash

WATCH_PERF="/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/.performance_list"
PROJECT_DIR="/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting"
CPP_INSERT="build/log_parser"

touch "${WATCH_PERF}"

while read -r FILE EVENT
do
    echo "INFO: ${LOG_TIME} Programme Info: Watch File: ${PERF_LOG}: ${EVENT}"
    PERF_LOG_FILE=$(cat ${FILE})
    ${SCRIPT_DIR}/${CPP_INSERT} insert performance ${PERF_LOG_FILE}
    echo "INFO: Log file ${PERF_LOG_FILE} Inserted into Database"    
done < <(inotifywait -q -e modify -m "${WATCH_PERF}")
