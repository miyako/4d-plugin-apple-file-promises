//%attributes = {}
C_TEXT:C284($1;$path;$2)
C_OBJECT:C1216($context)

$path:=$1
$context:=JSON Parse:C1218($2;Is object:K8:27)

CALL FORM:C1391(OB Get:C1224($context;"window";Is longint:K8:6);OB Get:C1224($context;"method";Is text:K8:3);$path)

