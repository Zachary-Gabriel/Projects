var express = require('express');
var router = express.Router();

router.get('/', function(req, res, next) {
	res.render('index', { title: 'Express' });
});

var lastTime = '';
var time = '';
router.get('/last.txt',function(req, res, next){
	time = Date();
	res.send(lastTime.toString());
	lastTime = time;
});

var colour = '';
var count = 0;
router.get('/colour.html',function(req, res, next){
	if(count%4 === 0){
		colour = 'red';
	} else if(count%4 === 1){
		colour = 'yellow';
	}else if(count%4 === 2){
		colour = 'green';
	}else if(count%4 === 3){
		colour = 'blue';
	}
	count++;
	res.send('<!DOCTYPE html>\
		<html>\
		<head>\
			<title>This is a title</title>\
		</head>\
		<body>\
			<h1 style="color:'+ colour +'" >'+ colour +'</h1>\
		</body>\
		</html>');
});

var array = {};
var count2 = 0;
var yeet = '';

router.get('/log.html',function(req, res, next){
	time = Date();
	count2++;
	array[count2-1] = time.toString();
	if (yeet == ''){
		yeet = '<li>' + array[count2-1] + '</li>';
	} else {
		yeet = yeet + '<li>' + array[count2-1] + '</li>';
	}

	res.send('<!DOCTYPE html>\
		<html>\
		<head>\
			<title>This is a title</title>\
		</head>\
		<body>\
			<ul>\
				'+yeet+'\
			</ul>\
		</body>\
		</html>');
});

var Counter3 = 0;
router.get('/first.html',function(req, res, next){
	Counter3++;
	if(Counter3 === 1){
		res.send('<!DOCTYPE html>\
		<html>\
		<head>\
			<title>This is a title</title>\
		</head>\
		<body>\
			<h1><a href = http://localhost:3000/main.html>Welcome</a></h1>\
			<p><a href = http://localhost:3000/main.html>random text</a></p>\
		</body>\
		</html>');
	} else {
		res.redirect('/main.html');
	}
});

router.get('/main.html',function(req, res, next){
	if(Counter3 != 0){
		Counter3++;
		res.send('<!DOCTYPE html>\
		<html>\
		<head>\
			<title>This is a title</title>\
		</head>\
		<body>\
			<h1>My main site</h1>\
			<p>random text</p>\
		</body>\
		</html>');
	} else {
		res.redirect('/first.html');
	}
});

var AjaxColour1 = '';
var AjaxCount1 = 0;
router.get('/colour.txt',function(req, res, next){
	if(AjaxCount1%4 === 0){
		AjaxColour1 = 'red';
	} else if(AjaxCount1%4 === 1){
		AjaxColour1 = 'yellow';
	}else if(AjaxCount1%4 === 2){
		AjaxColour1 = 'green';
	}else if(AjaxCount1%4 === 3){
		AjaxColour1 = 'blue';
	}
	AjaxCount1++;
	res.send(AjaxColour1);
});

router.get('/contact.ajax',function(req, res, next){
	res.send('<a href="a1724849@student.adelaide.edu.au">a1724849@student.adelaide.edu.au</a>');
});


router.get('/search.ajax',function(req, res, next){
	res.send('<input></input><button>search</button>');
});

module.exports = router;
