#!/bin/bash
#/**
#@file    fileslinks.sh
#@brief   Script de inicio para copiar ficheros y deinir enlaces simbólicos.
#@warning License: GNU GPLv3+
#@version 0.9
#@author  Ramon Gomez, ETSII Universidad de Sevilla
#@date    2009-10-10
#@version 1.0.5 - Enlace para librería libmac.
#@author  Ramon Gomez, ETSII Universidad de Sevilla
#@date    2012-06-28
#*/


# Si está configurado OpenGnSys ...
if [ -n "$OPENGNSYS" ]; then
    echo "${MSG_MAKELINKS:-.}"

    # Shell BASH por defecto (para usar "runtest")
    ln -fs /bin/bash /bin/sh

    # Enlace a la librería libmac para ld-mac.
    ln -fs $OGLIB/libmac.so /usr/lib

    # Crear directorio de bloqueos
    mkdir -p /var/lock 2>/dev/null || mkdir -p /run/lock

    # Crear ficheros temporales.
    touch $OGLOGCOMMAND $OGLOGCOMMAND.tmp $OGLOGSESSION /tmp/menu.tmp
    chmod 777 $OGLOGCOMMAND $OGLOGCOMMAND.tmp $OGLOGSESSION /tmp/menu.tmp

#    # Directorio de tipos de letras para el browser.
    QTDIR="/usr/local"
 #   mkdir -p $QTDIR/lib
  #  ln -fs $OGLIB/fonts $QTDIR/lib

else
    # FIXME Error: entorno de OpenGNSys no configurado.
    echo "Error: OpenGnSys environment is not configured."   # FIXME: definir mensaje.
    exit 1
fi

