var scan = new Array();

function demande_scan()
{
    var xhr = null;
    
    if (window.XMLHttpRequest) { 
        xhr = new XMLHttpRequest();
    } else if (window.ActiveXObject) {
        xhr = new ActiveXObject("Microsoft.XMLHTTP");
    }

    xhr.onreadystatechange = function() { reception_scan(xhr); };

    xhr.open("GET", "getScan.php", false);
    xhr.send(null);
}

function reception_scan(xhr)
{    
    var docXML= xhr.responseXML;
    var items = docXML.getElementsByTagName("mesure")

    for (i = 0; i < items.length; i++) 
	scan[i] (items.item(i).firstChild.data);
}