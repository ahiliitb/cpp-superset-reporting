#!/bin/bash

# syslog logging
THIS_PROCESS=$BASHPID
TAG="aggregator.db_insert"

# Redirect output to syslog
if [[ -t 1 ]]; then
    exec 1> >( exec logger --id=${THIS_PROCESS} -s -t "${TAG}" ) 2>&1
else
    exec 1> >( exec logger --id=${THIS_PROCESS} -t "${TAG}" ) 2>&1
fi


PROJECT_DIR="/home/ahilkhanjnv/MYWORK/safesquid/cpp-superset-reporting"
CPP_INSERT="build/log_parser"
CPP_CSP_CONVERT="build/csp_converter"

declare -A LOG_TYPE

LOG_TYPE["extended"]="extended"
LOG_TYPE["performance"]="performance"
LOG_TYPE["csp_converted"]="csp"


CONVERT_CSP () 
{
    for LOGFILE in ${PROJECT_DIR}/*/csp/*
    do
        LOG_DIR="$(dirname "${LOGFILE}")"
        LOG_FILE="$(basename "${LOGFILE}")"
        CSP_CONVERT_OUT="${LOG_DIR}_converted/${LOG_FILE}"
        [ ! -d "${LOG_DIR}_converted" ] && mkdir -p "${LOG_DIR}_converted" 
        ${SCRIPT_DIR}/${CPP_CSP_CONVERT} "${LOGFILE}"  "${CSP_CONVERT_OUT}"
    done
}

INSERT_INTO_DB () 
{
    for LOG in ${!LOG_TYPE[@]} 
    do 
        for LOGFILE in ${PROJECT_DIR}/*/${LOG}/*
        do
            LOG_DIR="$(dirname "${LOGFILE}")"
            ${SCRIPT_DIR}/${CPP_INSERT} insert ${LOG_TYPE[${LOG}]} "${LOGFILE}" 
        done
    done     
}

MAIN () 
{
    CONVERT_CSP
    INSERT_INTO_DB
}

MAIN