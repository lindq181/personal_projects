#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std;
int main() {
Matrix data = readMatrix("../samples/myData.dat");
int rows;

rows = numRows(data);

int cols;

cols = numCols(data);

int season_length;

season_length = 7;

int years;

years = ceil(((cols * 1.0) / season_length));

Matrix avgScore(rows,1);
 for(int i = 0; i < rows; i++){
	 for(rint j=0; j< 1; j++){
		 *(avgScore.access (i,j)) =  (({ Matrix pt(years,season_length);
 for(int i = 0; i < years; i++){
	 for(rint j=0; j< season_length; j++){
		 *(pt.access (i,j)) =  (({ int k;

k = ((i * season_length) + j);


if (k >= cols) then (0.0 - 25)
else data(row,k);})
);
 }
}
Matrix comparisonMatrix(years,years);
 for(int i = 0; i < years; i++){
	 for(rint j=0; j< years; j++){
		 *(comparisonMatrix.access (i,j)) =  (if (j <= i) then 0.0
else ({ float diff;

diff = 0;

int k;

for (k = 0;k <= (season_length - 1);k++ ){
diff = ((diff + pt(i,k)) - pt(j,k));


}


(diff / season_length);})
);
 }
}
Matrix modelAvgScore(years,1);
 for(int i = 0; i < years; i++){
	 for(rint j=0; j< 1; j++){
		 *(modelAvgScore.access (i,j)) =  (({ int x;

int y;

float score1;

score1 = 0.0;

for (x = 0;x <= yr;x++ ){
for (y = (yr + 1);y <= (years - 1);y++ ){
score1 = (score1 + comparisonMatrix(x,y));


}


}

score1 = ((score1 * 2) / ((yr * ((years - yr));
));
);

float score2;

score2 = 0.0;

for (x = 0;x <= yr;x++ ){
for (y = 0;y <= yr;y++ ){
score2 = (score2 + comparisonMatrix(x,y));


}


}

score2 = (score2 / (((((yr - 1));
 * yr) / 2));
);

float score3;

score3 = 0.0;

for (x = (yr + 1);x <= (years - 1);x++ ){
for (y = (yr + 1);y <= (years - 1);y++ ){
score3 = (score3 + comparisonMatrix(x,y));


}


}

score3 = (score3 / (((((years - yr));
 * (((years - yr) - 1));
) / 2));
);


((score1 - score2) - score3);})
);
 }
}
float maximum;

maximum = (0.0 - 25);

int k;

k = 0;

for (k = 0;k <= (years - 1);k++ ){
if ((modelAvgScore(k,0) > maximum)) 
	 {
maximum = modelAvgScore(k,0);


}

}


maximum;})
);
 }
}
int j;

for (j = 0;j <= (rows - 1);j++ ){
cout << "\n";


}


}
