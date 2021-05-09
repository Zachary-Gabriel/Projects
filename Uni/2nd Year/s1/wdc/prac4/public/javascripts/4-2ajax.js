//ajax
function changeColour() {

  // Create new AJAX request
  var xhttp = new XMLHttpRequest();
  
  // Define function that runs on response
  xhttp.onreadystatechange = function() {
  	if (this.readyState == 4 && this.status == 200) {
  		document.getElementsByTagName("h1")[0].style.color = this.responseText;
      document.getElementsByTagName("h1")[0].innerHTML = this.responseText;
    }
  };
  
  // Open connection
  xhttp.open("GET", "/colour.txt", true);
  
  // Send request
  xhttp.send();
  
} 
