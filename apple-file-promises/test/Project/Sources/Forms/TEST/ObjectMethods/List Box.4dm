If (Form event code:C388=On Double Clicked:K2:5)
	
	$Paths:=OBJECT Get pointer:C1124(Object named:K67:5;"Paths")
	$item:=$Paths->{$Paths->}
	
	If ($item#"")
		OPEN URL:C673($item)
	End if 
	
End if 