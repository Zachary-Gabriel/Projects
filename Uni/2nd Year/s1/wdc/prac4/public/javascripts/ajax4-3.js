function contact() {

  // Create new AJAX request
  var xhttp1 = new XMLHttpRequest();
  
  // Define function that runs on response
  xhttp1.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("myDIV").innerHTML = this.responseText;
    }
  };
  
  // Open connection
  xhttp1.open("GET", "/contact.ajax", true);
  
  // Send request
  xhttp1.send();
  
}
function search() {

  // Create new AJAX request
  var xhttp2 = new XMLHttpRequest();
  
  // Define function that runs on response
  xhttp2.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("myDIV").innerHTML = this.responseText;
    }
  };
  
  // Open connection
  xhttp2.open("GET", "/search.ajax", true);
  
  // Send request
  xhttp2.send();
  
}
function about() {

  // Create new AJAX request
  var xhttp3 = new XMLHttpRequest();
  
  // Define function that runs on response
  xhttp3.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
    document.getElementById("myDIV").innerHTML = this.responseText;
   }
 };

  // Open connection
  xhttp3.open("GET", "/about.ajax", true);
  
  // Send request
  xhttp3.send();
  
}