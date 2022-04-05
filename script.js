var firebaseConfig = {
    apiKey: "AIzaSyB9wzeGiUpNLnvnKUoXirbXPMsWh70C35U",
    authDomain: "esp32-webserver-bab81.firebaseapp.com",
    databaseURL: "https://esp32-webserver-bab81-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "esp32-webserver-bab81",
    storageBucket: "esp32-webserver-bab81.appspot.com",
    messagingSenderId: "310779774493",
    appId: "1:310779774493:web:16cde329075e93eb55e8f4",
    measurementId: "G-K92G0EHZTJ"
  };
  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);

$(document).ready(function(){
    var database = firebase.database();
	var LedStatus;

	database.ref().on("value", function(snap){
		var t = document.getElementById("temp");
		var p = document.getElementById("press");
		var a = document.getElementById("alt");
		t.innerHTML = snap.val().Temperature;        
		p.innerHTML = snap.val().Pressure;          
		a.innerHTML = snap.val().Altitude;
		LedStatus = snap.val().LedStatus;
		if(LedStatus == "0"){    // check from the firebase
			//$(".Light1Status").text("The light is off");
			document.getElementById("unact").style.display = "none";
			document.getElementById("act").style.display = "block";
		} else {
			//$(".Light1Status").text("The light is on");
			document.getElementById("unact").style.display = "block";
			document.getElementById("act").style.display = "none";
		}
	});
    $(".toggle-btn").click(function(){
		var firebaseRef = firebase.database().ref().child("LedStatus");

		if(LedStatus == "1"){    // post to firebase
			firebaseRef.set("0");
			LedStatus = "0";
		} else {
			firebaseRef.set("1");
			LedStatus = "1";
		}
	})
});