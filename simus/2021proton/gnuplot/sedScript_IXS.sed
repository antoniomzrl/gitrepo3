# Script para convertir los ficheros IXS de la simulaci�n 2016proton a un
# formato compatible con mi rutina de gnuplot
#
# Ejemplos de uso:
# sed -f sedScript_IXS.sed wlp-IXS-2015dic_ORIGINAL.txt > wlp-IXS-2015dic.txt
#
# sed -i -f sedScript.sed_IXS wlp-IXS-2015dic_ORIGINAL.txt
# CUIDADO: Con la opci�n -i no se crea una copia del fichero original, sino que se sobreescribe


#Reemplazo los espacios por una coma
s/\s\+/,/g

#Elimino las comas del comienzo de l�nea
s/^,//g

#Elimino las comas del final de l�nea
s/,$//g

#Elimino la coma detr�s de una almohadilla
s/^#,/#/g