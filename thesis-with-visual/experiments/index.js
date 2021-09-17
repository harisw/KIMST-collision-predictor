var classifyPoint = require("robust-point-in-polygon")
// var Points = [	//Radarcircle
//   {x:1272857, y:392882},
//   {x:1215564, y:216545},
//   {x:1065565, y:107562},
//   {x:880154, y:107560},
//   {x:730153, y:216540},
//   {x:672857, y:392875},
//   {x:730150, y:569212},
//   {x:880150, y:678194},
//   {x:1065561, y:678195},
//   {x:121556, y:569213}
// ]
var Points = [
	{x: 972857, y:392878},
	{x: 831436, y:251455},
	{x: 972859, y:192878},
	{x: 1114280, y:251458}
]
function onClick() {
    Points.forEach((point, index, arr) => {
	  console.log(index)
	  const line = document.createElementNS('http://www.w3.org/2000/svg', 'line');
	  const ratio = 5000;
	  if(index == (Points.length-1)){
        line.setAttribute('x1', point.x/ratio);
        line.setAttribute('y1', point.y/ratio);
        line.setAttribute('x2', Points[0].x/ratio);
        line.setAttribute('y2', Points[0].y/ratio);
        //line.setAttribute('fill', 'red');
	  }
	  else {
		line.setAttribute('x1', point.x/ratio);
        line.setAttribute('y1', point.y/ratio);
        line.setAttribute('x2', Points[index+1].x/ratio);
        line.setAttribute('y2', Points[index+1].y/ratio);
        //line.setAttribute('fill', 'red');
		}
		line.setAttribute('style', "stroke:rgb(255,0,0);stroke-width:2");
		document.getElementById('svg').appendChild(line);
	});
}

var polygon = [ [ 1, 1 ], [ 1, 2 ], [ 2, 2 ], [ 2, 1 ] ]

console.log(
  classifyPoint(polygon, [1.5, 1.5]),
  classifyPoint(polygon, [1, 2]),
  classifyPoint(polygom, [100000, 10000]))