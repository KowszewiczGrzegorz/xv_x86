var scan;

function demande_scan()
{
    var xhr = null;
    
    if (window.XMLHttpRequest) { 
        xhr = new XMLHttpRequest();
    } else if (window.ActiveXObject) {
        xhr = new ActiveXObject("Microsoft.XMLHTTP");
    }

    xhr.open("GET", "scan.txt", false);
    xhr.send(null);
    
    scan = xhr.responseText;
}

