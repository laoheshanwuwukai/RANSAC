#include "RANSAC.h"

template<typename T , typename S>
double RANSAC<T,S>::compute(std::vector<S> & parameters , 
        ParameterEstimator<T , S>* paramEstimator,
        std::vector<T>& data , 
        double desiredProabilityForOutliers){

    int numDataObjects = data.size();
    unsigned int numForEstimate = paramEstimator->numForEstimate();

    //Special case
    if(numDataObjects < numForEstimate || 
        desiredProabilityForOutliers<=0.0||
        desiredProabilityForOutliers>=1.0) 
    return 0;
    
    std::vector<T*> exactEstimateData;
    std::vector<T*> leastSquaresEstimateData;
    std::vector<S> exactEstimateParameters;
    // for loop i, j , k , l ;
    // numVotesForBest/numVotesForCur use to decide the best Subset
    //maxIndex use to random select
    int i , j , k , l , numVotesForBest , numVotesForCur, maxIndex;
    unsigned int numTries;
    //true if data[i] agrees with the best model
    bool* bestVotes = new bool[numDataObjects];
    //true if data[i] agrees with the current model;
    bool* curVotes = new bool[numDataObjects];
    //true if data[i] is not chosen for computing the exact fit;
    bool* notChosen = new bool[numDataObjects];
    //Use to order the set 
    SubSetIndexComparator subSetIndexComparator(numForEstimate);
    std::set<int* ,  SubSetIndexComparator> chosenSubSets(subSetIndexComparator);
    // the chosen subset to create the model
    int * curSubSetIndexes; 
    /*
        k value in paper(pick time)(numTries);
        (1-z) = (1-w^n)^k
        k = log(1-z) / log(1-w^n)
    */
    double numerator = log(1.0 - desiredProabilityForOutliers);
    double denominator;
    //Cause we don't know the w ,set numTries to max;
    unsigned int allTries = choose(numDataObjects , numForEstimate);

    parameters.clear();
    srand((unsigned)time(NULL));

    numVotesForBest = 0;
    numTries = allTries;

    for(i = 0 ; i<numTries ; i++){
        //randomly select data for exact model fit
        std::fill(notChosen , notChosen+numDataObjects , true);
        curSubSetIndexes = new int[numForEstimate];

        exactEstimateData.clear();
        maxIndex = numDataObjects - 1;

        for(l = 0 ; l<numForEstimate ; l++){
            //Use rand() function to generate random coffient
            int selectedIndex = 
                (int)( ( (double)rand()/(double)RAND_MAX)*maxIndex +0.5 );
            //
            for(j = -1 , k = 0; k<numDataObjects && j<selectedIndex ; k++ ){
                if(notChosen[k]) j++;
            }
            k--;
            exactEstimateData.push_back(&(data[k]));
            notChosen[k] = false;
            maxIndex--; //TODO why??
        
        }//end for pick numForEstimate points
        //get subSet Index chosen before;
        for(l = 0 , j = 0; j<numDataObjects ; j++){
            if(!notChosen[j]){
                curSubSetIndexes[l] = j+1;//TODO why j+1;
                l++;
            }
        }
        std::pair<typename std::set<int* , SubSetIndexComparator>::iterator , bool> res = chosenSubSets.insert(curSubSetIndexes);
        if(res.second == true){//first time we chose this sub set
            paramEstimator->estimate(exactEstimateData , exactEstimateParameters);
            //fail condition
            if(exactEstimateParameters.size() == 0) continue;
            //Votes for this sub Set
            numVotesForCur = 0;
            std::fill(curVotes , curVotes+numDataObjects , false);
            //
            for(j = 0 ; j<numDataObjects && numVotesForBest - numVotesForCur<numDataObjects-j+1 ;j++){
                if(paramEstimator->agree(exactEstimateParameters , data[j])){
                    curVotes[j] = true;
                    numVotesForCur++;
                }
            }

            if(numVotesForCur > numVotesForBest){
                numVotesForBest = numVotesForCur;
                std::copy(curVotes , curVotes+numDataObjects , bestVotes);
                if(numVotesForBest == numDataObjects)
                    i = numTries;
                else{ //Update the setimate of outliers
                    denominator = log(
                        1.0 - pow((double)numVotesForCur/(double)numDataObjects , 
                        (double)(numForEstimate)));
                    numTries = (int)(numerator/denominator + 0.5);
                    numTries = numTries<allTries ? numTries : allTries;
                }
            }

        } // end of SubSet selected successful
        else{ // sub set already appeared , release memory
            delete [] curSubSetIndexes;
        }

    }//end for numTries


    typename std::set<int* , SubSetIndexComparator>::iterator it = chosenSubSets.begin();
    typename std::set<int* , SubSetIndexComparator>::iterator chosenSetEnd = chosenSubSets.end();

    while(it!=chosenSetEnd){
        delete [] (*it);
        it++;
    }
    chosenSubSets.clear();

    if(numVotesForBest>0){
        for(j=0; j <numDataObjects;j++){
            if(bestVotes[j]) leastSquaresEstimateData.push_back(&(data[j]));
        }
        paramEstimator->leastSquaresEstimate(leastSquaresEstimateData , parameters);

    }
    delete [] bestVotes;
    delete [] curVotes;
    delete [] notChosen;

    return (double)numVotesForBest/(double)numDataObjects;
}


/*****************************************************************************/

template<typename T, typename S>
unsigned int RANSAC<T,S>::choose(unsigned int n, unsigned int m)
{
	double denominatorEnd, numeratorStart, numerator,denominator, i, result; 
        //perform smallest number of multiplications
	if((n-m) > m) {
		numeratorStart = n-m+1;
		denominatorEnd = m;
	}
	else {
		numeratorStart = m+1;
		denominatorEnd = n-m;
	}
	
	for(i=numeratorStart, numerator=1; i<=n; i++)
		numerator*=i;
	for(i=1, denominator=1; i<=denominatorEnd; i++)
		denominator*=i;	
	result = numerator/denominator;
	
	         //check for overflow both in computation and in result	         
	if(denominator>std::numeric_limits<double>::max() || 
	   numerator>std::numeric_limits<double>::max() || 
	   static_cast<double>(std::numeric_limits<unsigned int>::max())<result )	
	  return std::numeric_limits<unsigned int>::max();
	else 
	  return static_cast<unsigned int>(result);   
}
