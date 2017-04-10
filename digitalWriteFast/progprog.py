import os

output = ""

def writeFunct(items,output):
  a=items[0]
  c=items[2]
  b=items[3]
  if ((a<15) or ( (a>=44) and (a<=46))):
        c= 254
        if (highlow1 == 'LOW'):
          c=1
        output=output+'analogWrite('+str(a)+','+str(c)+');\n'
  if ((b<15) or ( (b>=44) and (b<=46))):
        for i in range(len(txt)):  
          output += txt[i]+ str(items[i])
        c= 254
        if (highlow1 == 'LOW'):
          c=1
        output=output+'analogWrite('+str(b)+','+str(c)+');\n'
        for i in range(len(txt)):  
            output += txt[i]+ str(items[i])
  for i in range(len(txt)):  
    output += txt[i]+ str(items[i])
  if ((a<15) or ( (a>=44) and (a<=46))):
        for i in range(len(txt)):
            output += txt2[i]+ str(items[i])
        c= 254
        if (highlow1 == 'LOW'):
          c=1
        output=output+'analogWrite('+str(a)+','+str(c)+');\n'
        for i in range(len(txt)):
            output += txt2[i]+ str(items[i])
  if ((b<15) or ( (b>=44) and (b<=46))):
        c= 254
        if (highlow1 == 'LOW'):
          c=1
        output=output+'analogWrite('+str(b)+','+str(c)+');\n'
  for i in range(len(txt)):
    output += txt2[i]+ str(items[i])
  return output
  
def writeToDisk (text):
   print (text)
   #tm = open('~/Documents/Arduino/dWFast.text','w')
   #tm.write( text )                        # write out the program text
   #tm.close()
   return 
   
txt = ('pinModeFast(',',INPUT);\ndigitalWriteFast(',',','); \npinModeFast(',',OUTPUT);\ndigitalWriteFast(',',',');\ndelay(1);\nif((int) digitalReadFast(',') != ',') error(',',',',',');\n')
txt2 = ('pinModeFast2(',',INPUT);\ndigitalWriteFast2(',',',');\npinModeFast2(',',OUTPUT);\ndigitalWriteFast2(',',',');\ndelay(1);\nif((int) digitalReadFast2(',') != ',') error(',',',',',');\n')
out=""
for j in (0,6,12):
    for i in (0,1,2):
      highlow1='HIGH'
      highlow2='LOW'
      a = 2+i+j
      b = 5 + i+j
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,1,"\n"),out)
      (a,b) = (b,a)
      out= writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,2,"\n"),out)
      (highlow1,highlow2) = (highlow2,highlow1)
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,3,"\n"),out)
      (a,b) = (b,a)
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,4,"\n"),out)
out = out +"\n//now do the back row pins on the Mega\n\n"
for j in (0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32):
      highlow1='HIGH'
      highlow2='LOW'
      a = 20 +  j
      b = 21 + j
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,1,"\n"),out)
      (a,b) = (b,a)
      out= writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,2,"\n"),out)
      (highlow1,highlow2) = (highlow2,highlow1)
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,3,"\n"),out)
      (a,b) = (b,a)
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,4,"\n"),out)
for j in (0,6,12):
   for i in (0,1,2):
      highlow1='HIGH'
      highlow2='LOW'
      a = 54+i+j
      b = 57+ i+j
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,1,"\n"),out)
      (a,b) = (b,a)
      out= writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,2,"\n"),out)
      (highlow1,highlow2) = (highlow2,highlow1)
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,3,"\n"),out)
      (a,b) = (b,a)
      out = writeFunct((a,a,highlow1,b,b,highlow2,a,highlow2,a,b,4,"\n"),out)
writeToDisk(out)

  

   
