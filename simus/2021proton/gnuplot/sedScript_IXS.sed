# Script para convertir los ficheros IXS de la simulación 2016proton a un
# formato compatible con mi rutina de gnuplot
#
# Ejemplos de uso:
# sed -f sedScript_IXS.sed wlp-IXS-2015dic_ORIGINAL.txt > wlp-IXS-2015dic.txt
#
# sed -i -f sedScript.sed_IXS wlp-IXS-2015dic_ORIGINAL.txt
# CUIDADO: Con la opción -i no se crea una copia del fichero original, sino que se sobreescribe


#Reemplazo los espacios por una coma
s/\s\+/,/g

#Elimino las comas del comienzo de línea
s/^,//g

#Elimino las comas del final de línea
s/,$//g

#Elimino la coma detrás de una almohadilla
s/^#,/#/g