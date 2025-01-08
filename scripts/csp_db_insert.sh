#!/bin/bash

WATCH_CSP="/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting/.csp_list"
PROJECT_DIR="/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting"
CPP_INSERT="build/log_parser"

touch "${WATCH_CSP}"

while read -r FILE EVENT
do
    echo "INFO: ${LOG_TIME} Programme Info: Watch File: ${PERF_LOG}: ${EVENT}"
    CSP_LOG_FILE=$(cat ${FILE})
    ${SCRIPT_DIR}/${CPP_INSERT} insert csp ${CSP_LOG_FILE}
    echo "INFO: Log file ${CSP_LOG_FILE} Inserted into Database"    
done < <(inotifywait -q -e modify -m "${WATCH_CSP}")
