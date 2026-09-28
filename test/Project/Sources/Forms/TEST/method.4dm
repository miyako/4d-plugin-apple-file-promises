Case of 
	: (Form event code:C388=On Load:K2:1)
		
		$method:="CB"
		$context:=JSON Stringify:C1217(New object:C1471("window";Current form window:C827;"method";"CBF"))
		
		ACCEPT FILE PROMISES (1;$method;$context)
		
	: (Form event code:C388=On Unload:K2:2)
		
		ACCEPT FILE PROMISES (0)
		
End case 