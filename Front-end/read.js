
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
/////////////////////////////////////////////////
firebase.initializeApp(firebaseConfig);

// Retreaving data from firebase
/////////////////////////////////////////////////
var rollidRef = firebase.database().ref('MAS/1');
var no_of_rolls = 0;

// //////////////////////////////////////////
rollidRef.on('value', gotData1, hasSubNodes);

rollidRef.on('value', hasSubNodes);
function hasSubNodes(data){
  var mainNodes = data.val();
  var keys = Object.keys(mainNodes);
  var values = Object.values(mainNodes);
  console.log("object values = " + values[1]);

  var n = 0;
  for(var i=0;i<keys.length;i++){
    if(values[i] != "[object Object]"){
      n++;
    }
  }
console.log("n = "+ n); 
  no_of_rolls =  n;
}

console.log("no_of_rolls = "+ no_of_rolls)

// TRY TO FIGURE OUT THE NECCESARY CODE RIGHT HERE6


// /////////////////////////////////////////


rollidRef.on('value', gotData1);
function gotData1(data) {
  var mainNodes = data.val();
  var keys = Object.keys(mainNodes);
  var values = Object.values(mainNodes);

  for(var i=0;i<keys.length-9; i++){
    var k = keys[i];
    var v = values[i];

    var dam_len = mainNodes[k].damage_length;
    if(dam_len == null) dam_len = 0.00;
    
    var overlapped_len = mainNodes[k].overlap_length;
    if(overlapped_len == null) overlapped_len = 0.00;
  
    var plies = mainNodes[k].plies;
    if(plies == null) plies = 0;

    var update_total_used_len = mainNodes[k].update_total_used_length;
    if(update_total_used_len == null) update_total_used_len = 0.00;

    var comment = mainNodes[k].comment;
    if(comment == null) comment = 0.00;

    var end_len = mainNodes[k].end_length;
    if(end_len == null) end_len = 0.00;
    
    // var dam_len = mainNodes[k].damage_length;
    // var overlapped_len = mainNodes[k].overlap_length;
    // var plies = mainNodes[k].plies;
    // var tktlen = mainNodes[k].tktlen;
    // var update_total_used_len = mainNodes[k].update_total_used_length;
    // var comment = mainNodes[k].comment;
    // var end_len = mainNodes[k].end_length;

    document.getElementById("rollId").innerHTML = k;
    document.getElementById("commentVal").innerHTML = comment;
    document.getElementById("pliesVal").innerHTML = plies;
    document.getElementById("damageVal").innerHTML = dam_len;
    document.getElementById("overlapVal").innerHTML = overlapped_len;
    document.getElementById("usedVal").innerHTML = update_total_used_len;
    // document.getElementById("tktlenVal").innerHTML = tktlen_;
    document.getElementById("endlenVal").innerHTML = end_len;
  }
}
// The following are the common parameters
// ///////////////////////////////////////////////////////

// var batch_damage_lenRef = firebase.database().ref('MAS/1/batch_damage_length');
// var batch_end_lenRef = firebase.database().ref('MAS/1/batch_end_length');
// var batch_overlap_lenRef = firebase.database().ref('MAS/1/batch_overlap_length');
var batch_total_pliesRef = firebase.database().ref('MAS/1/batch_total_plies');

// var batch_used_lenRef = firebase.database().ref('MAS/1/batch_used_length');
var ontrackRef = firebase.database().ref('MAS/1/on_track');
var user_input_layer_lenRef = firebase.database().ref('MAS/1/user_input_layer_length');
var user_input_rollsRef = firebase.database().ref('MAS/1/user_input_no_of_rolls');
var user_input_pliesRef = firebase.database().ref('MAS/1/user_input_number_of_plies');


// damage
// batch_damage_lenRef.on('value', gotData2);
//  function gotData2(data) {
//    var damageLen = data.val();
//    console.log(damageLen);
//    document.getElementById("batch_damage_lenId").innerHTML = damageLen;
//  }

// Layer len
// batch_end_lenRef.on('value', gotData3);
//  function gotData3(data) {
//    var layerlen = data.val();
//    document.getElementById("batch_end_lenId").innerHTML = layerlen;
//  }

// Total overlap length
// batch_overlap_lenRef.on('value', gotData4);
//  function gotData4(data) {
//    var t_overLap = data.val();
//    document.getElementById("batch_overlap_lenId").innerHTML = t_overLap;
//  }

// Total plies
batch_total_pliesRef.on('value', gotData5);
function gotData5(data) {
  var t_plies = data.val();
  document.getElementById("batch_total_pliesId").innerHTML = t_plies;
}

// Total used length
// batch_used_lenRef.on('value', gotData6);
// function gotData6(data) {
//   var t_used = data.val();
//   document.getElementById("batch_used_lenId").innerHTML = t_used;
// }

// Total overlap length
ontrackRef.on('value', gotData7);
function gotData7(data) {
  var isOntrack = data.val();
  
  // console.log(isOntrack);
  console.log(isOntrack);

  if(isOntrack == 1){
    document.getElementById("ontrackId_true").style.display = "block";
    document.getElementById("ontrackId_false").style.display = "none";
  }else{
    document.getElementById("ontrackId_true").style.display = "none";
    document.getElementById("ontrackId_false").style.display = "block";
  }
}

// input layer length
user_input_layer_lenRef.on('value', gotData8);
function gotData8(data) {
  var inputLayerLen = data.val();
  document.getElementById("user_input_layer_lenId").innerHTML = inputLayerLen;
}

// Input roll number
// user_input_rollsRef.on('value', gotData9);
// function gotData9(data) {
//   var inputRolls = data.val();

//   console.log("input rolls = "+inputRolls);
//   // if(inputRolls !== null){
//     document.getElementById("user_input_rollsId").innerHTML = inputRolls;
//   // }
//   // document.getElementById("user_input_rollsId").innerHTML = inputRolls;
// }

// input Plies
user_input_pliesRef.on('value', gotData10);
function gotData10(data) {
  var inputPlies = data.val();
  document.getElementById("user_input_pliesId").innerHTML = inputPlies;
}

