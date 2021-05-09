//ajax
function change() {

  // Create new AJAX request
  var xhttp = new XMLHttpRequest();
  
  // Define function that runs on response
  xhttp.onreadystatechange = function() {
  	if (this.readyState == 4 && this.status == 200) {
  		document.getElementById("paragraph").innerHTML = "This page was last viewed " + this.responseText;
  	}
  };
  
  // Open connection
  xhttp.open("GET", "/last.txt", true);
  
  // Send request
  xhttp.send();
  
} 
