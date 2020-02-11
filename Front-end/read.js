
var firebaseConfig = {
  apiKey: "AIzaSyCfbDEvntAjCft006d_t9fQuNwuVHmBl3s",
  authDomain: "masdemo-146c2.firebaseapp.com",
  databaseURL: "https://masdemo-146c2.firebaseio.com",
  projectId: "masdemo-146c2",
  storageBucket: "masdemo-146c2.appspot.com",
  messagingSenderId: "253329309704",
  appId: "1:253329309704:web:376b0267bee7a9dad411bd",
  measurementId: "G-XKDMFRXJBF"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
// firebase.analytics();

// var batchRef = firebase.database().ref('MAS/1/');
// var rollidRef = firebase.database().ref('MAS/1/');
// var tktlenRef = firebase.database().ref('MAS/1/'+ {rollId});

// var tdamageRef = firebase.database().ref('MAS/1/'+{rollId});
// // var tpliesRef = firebase.database().ref('MAS/1/'+ {rollId});
// var tusedRef = firebase.database().ref('MAS/1/'+ {rollid});




// var comment = firebase.database().ref('messages/cccc/a2');


// Retreaving data from firebase
/////////////////////////////////////////////////

// batchRef.on('value', gotData2);
// function gotData2(data) {
  // var scores = data.val();
  // document.getElementById("batchVal").innerHTML = 1;
// }

// Retreaving data from firebase
/////////////////////////////////////////////////
document.getElementById("batchVal").innerHTML = 1;

var rollidRef = firebase.database().ref('MAS/1');

rollidRef.on('value', gotData1);


function gotData1(data) {
  var mainNodes = data.val();
  var keys = Object.keys(mainNodes);
  var values = Object.values(mainNodes);

  console.log("the values are "+values);

  console.log("the mainNodes are "+mainNodes);
  console.log("the keys are "+keys);

  for(var i=0;i<keys.length-4; i++){
    var k = keys[i];
    var v = values[i];
    console.log(k);
    console.log(v);
    //  var a = parseInt(k);
    //  console.log("a = "+typeof(a));

    // var onTrack = Object.values(mainNodes[k].on_track);
    var onTrack = v.on_track;
     console.log("on track: "+onTrack)
    var plies = v.plies;
    //  console.log(plies)
    var total_dam_len = mainNodes[k].total_damage_length;
    //  console.log("dam len= "+ total_dam_len)
    var total_overlapped_len = mainNodes[k].total_overlapped_length;
    var update_total_used_len = mainNodes[k].update_total_used_length;
    
    document.getElementById("rollId").innerHTML = k;
    document.getElementById("ontrackVal").innerHTML = onTrack;
    document.getElementById("totalPliesVal").innerHTML = plies;
    document.getElementById("damageVal").innerHTML = total_dam_len;
    document.getElementById("overlapVal").innerHTML = total_overlapped_len;
    document.getElementById("usedVal").innerHTML = update_total_used_len;
  }
}



 var tktlenRef = firebase.database().ref('MAS/1/tktlen');
 var tpliesRef = firebase.database().ref('MAS/1/updated_total_plies');
 var layerLen = firebase.database().ref('MAS/1/user_input_layer_length');
 var toverlapRef = firebase.database().ref('MAS/1/user_input_number)of_plies');
 var endRef = firebase.database().ref('MAS/1/user_input_number_of_rolls');

 var tusedRef = firebase.database().ref('MAS/1/user_input_number_of_rolls');
var tdamageRef = firebase.database().ref('MAS/1/user_input_number_of_rolls');

// // tktlen
// var tktlen = 0;
// tktlenRef.on('value', gotData);
// function gotData(data) {
//   var tktlen = data.val();
//   document.getElementById("tktlenVal").innerHTML = tktlen;
// }


// total plies
 tpliesRef.on('value', gotData);
 function gotData(data) {
   var plies = data.val();
   document.getElementById("totalPliesVal").innerHTML = plies;
 }

// Layer len
 layerLen.on('value', gotData);
 function gotData(data) {
   var layerlen = data.val();
   document.getElementById("layerlenVal").innerHTML = layerlen;
 }

// Total overlap length
 toverlapRef.on('value', gotData);
 function gotData(data) {
   var t_overLap = data.val();
   document.getElementById("overlapVal").innerHTML = t_overLap;
 }

//  
//  endRef.on('value', gotData);
//  function gotData(data) {
//    var mainNodes = data.val();
//    document.getElementById("ontrackVal").innerHTML = scores;
//  }

 
 

let comment_cal = ( totalPliesVal,layerLen,used,damage,overlap,ends) => {
  ((totalPliesVal*layerLen)+used+damage+overlap+ends)
 }
 
 let comment = 0;
 comment = comment_cal(tktlenRef, tpliesRef, layerLen, tusedRef, tdamageRef, toverlapRef, endRef);
 
 document.getElementById("comment").innerHTML = comment;

/**
 *  var batchRef = firebase.database().ref('messages/aaaa/a1');
    var rollidRef = firebase.database().ref('messages/bbbb/aa1');
    var tktlenRef = firebase.database().ref('messages/bbbb/aa2');
    var tpliesRef = firebase.database().ref('messages/cccc/a1');
    var layerLen = firebase.database().ref('messages/cccc/a1');
    var tdamageRef = firebase.database().ref('messages/cccc/a2');
    var toverlapRef = firebase.database().ref('messages/cccc/a2');
    var tusedRef = firebase.database().ref('messages/cccc/a2');
    var endRef = firebase.database().ref('messages/cccc/a2');
 */




// Retreaving data from firebase
/////////////////////////////////////////////////



// Retreaving data from firebase
/////////////////////////////////////////////////

// tktlenRef.on('value', gotData3);

// function gotData3(data) {
  // var scores = data.val();
  // console.log(data.val());
  
// }

// Retreaving data from firebase
/////////////////////////////////////////////////

// tpliesRef.on('value', gotData4); //add err at last as arg

// function gotData4(data) {
  // var scores = data.val();
  
// }

// Retreaving data from firebase
/////////////////////////////////////////////////

// tdamageRef.on('value', gotData5);

// function gotData5(data) {
  // var scores = data.val();
  
// }


// Retreaving data from firebase
/////////////////////////////////////////////////

// toverlapRef.on('value', gotData6);

// function gotData6(data) {
//   // var scores = data.val();
  
// }


// // Retreaving data from firebase
// /////////////////////////////////////////////////

// tusedRef.on('value', gotData7);

// function gotData7(data) {
//   // var scores = data.val();
  
// }


