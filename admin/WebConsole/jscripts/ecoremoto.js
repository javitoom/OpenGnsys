//______________________________________________________________________________________________________
function enviaping(){ 
	var idambito=document.fdatos.idambito.value;
	var litambito=document.fdatos.litambito.value;
	var wurl="ecoconsola.php";
	var prm="idambito="+idambito+"&litambito="+litambito;
	CallPage(wurl,prm,"retorno","POST");

}
//______________________________________________________________________________________________________
function retorno(iHTML){
	if(iHTML.length>0){
		var diveco=document.getElementById("diveco");
		diveco.innerHTML="<PRE>"+iHTML+"</PRE>";
		//setTimeout('conmuta("hidden");',300);
		conmuta("hidden");
	}
	setTimeout("enviaping();",5000); 
}
//______________________________________________________________________________________________________
function conmuta(estado){
	var layavi=document.getElementById("layer_aviso");
	layavi.style.visibility=estado;
}
