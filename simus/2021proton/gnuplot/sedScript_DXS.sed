# Script para convertir los ficheros DXS de la simulaci�n 2016proton a un
# formato compatible con mi rutina de gnuplot
#
# Ejemplos de uso:
# sed -f sedScript_DXS.sed water_liquid_DXSSEC_proton_ORIGINAL.txt > water_liquid_DXSSEC_proton.txt
# 
# sed -i -f sedScript_DXS.sed water_liquid_DXSSEC_proton_ORIGINAL.txt
# CUIDADO: Con la opci�n -i no se crea una copia del fichero original, sino que se sobreescribe


#Reemplazo los espacios por una coma
s/\s\+/,/g

#Elimino las comas del comienzo de l�nea a partir de la tercera l�nea
3,${s/^,//g}

#Elimino las comas del final de l�nea
s/,$//g