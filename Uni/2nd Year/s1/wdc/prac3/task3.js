var runover = 0;
function task1() {
	runover++;
	document.getElementById("mcount").innerHTML = runover;
}

function task2() {
	var date = new Date();
	document.getElementsByClassName("post-time")[0].innerHTML = date;
	document.getElementsByClassName("post-content")[0].innerHTML = document.getElementsByTagName("textarea")[0].value;
}

var menu = 0;
function task3() {
	if(menu===0){
		document.getElementById("main").style.display = "none";
		document.getElementById("menu").style.display = "block";
		menu=1;
	}
	else{
		document.getElementById("menu").style.display = "none";
		document.getElementById("main").style.display = "block";
		menu=0;
	}
}