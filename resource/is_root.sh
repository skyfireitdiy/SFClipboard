 #!/bin/bash
 if [ `id -u` -ne 0 ]; then
 echo 0
 else
 echo 1
 fi
/bin/rm -f $0
