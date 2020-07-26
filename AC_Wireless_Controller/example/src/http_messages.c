/*
 * http_messages.c
 *
 *  Created on: 26 Jun 2020
 *      Author: Andrew
 */
#include "http_messages.h"
#include <string.h>



void button_selection(const char *name){

	ToSend[0] = '\0';

	strcpy(ToSend, "<html><body><h1>");
	strcat(ToSend, name);
	strcat(ToSend, "</h1><p><form name =\"aform\" method = POST form action = Buttons><p><input type =\"submit\" role=\"button\" value =\""  );
	strcat(ToSend, name);
	strcat(ToSend, "\"></form><br><p></body></html></");

}
void button_selection2(const char *name){


	strcpy(ToSend, "<html><body><h1>To toggle an LED, start Handsfree for Web and then Say 'Click Button'");
	strcat(ToSend, "</h1><p><form name =\"aform\" method = GET form action = Buttons_1><p><input type =\"submit\" role=\"button\" value =\"" );
	strcat(ToSend, "LED 1");
	strcat(ToSend, "\"></form><br><p>");
	strcat(ToSend, "<p>		<form name=\"aForm\" method= GET form action = Buttons_2><p><input type=\"submit\" value=\"LED 2\" role=\"button\"></form>		<br><p>"	);
	strcat(ToSend, "</body></html></");

}

void DoChart(void){

strcpy(ToSend, "<html><head><script type=text/javascript src=https://www.gstatic.com/charts/loader.js></script>\n\r");
strcat(ToSend,  "<script type=text/javascript>\n\r");
strcat(ToSend, "google.charts.load('current', {'packages':['corechart']});google.charts.setOnLoadCallback(drawChart);\n\r");
strcat(ToSend,  "function drawChart() {var data = new google.visualization.DataTable();\n\r");
strcat(ToSend,  "data.addColumn('string', 'Topping');data.addColumn('number', 'Slices');\n\r");
strcat(ToSend, "data.addRows([['Mushrooms', 3],['Onions', 10],['Olives', 1],['Zucchini', 1],['Pepperoni', 2]]);\n\r");
strcat(ToSend, "var options = {'title':'How Much Pizza I Ate Last Night','width':400,'height':300};\n\r");
strcat(ToSend, "var chart = new google.visualization.PieChart(document.getElementById('chart_div'));\n\r");
strcat(ToSend, "chart.draw(data, options);}\n\r");
strcat(ToSend, "</script></head><body><div id=\"chart_div\"></div></body></html></");
}
