// This code is a modification of the code from http://robotics.hobbizine.com/arduinoann.html


#include <arduino.h>
#include "neural_network.h"
#include <math.h>

void NeuralNetwork::initialize(float LearningRate, float Momentum) {
    this->LearningRate = LearningRate;
    this->Momentum = Momentum;
}

void NeuralNetwork::initWeights() {
    for(int i = 0 ; i < HiddenNodes ; i++ ) {    
        for(int j = 0 ; j <= InputNodes ; j++ ) { 
            ChangeHiddenWeights[j*HiddenNodes + i] = 0.0 ;
            float Rando = float(random(100))/100;
            HiddenWeights[j*HiddenNodes + i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
        }
    }

    for(int i = 0 ; i < OutputNodes ; i ++ ) {    
        for(int j = 0 ; j <= HiddenNodes ; j++ ) {
            ChangeOutputWeights[j*OutputNodes + i] = 0.0 ;  
            float Rando = float(random(100))/100;        
            OutputWeights[j*OutputNodes + i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
        }
    }
}

// void softmax(float input_output_data, int classes){
// 	double sum = 0;
//     float output_data[classes] = {};

// 	for(int j = 0; j < classes; j++){
// 		output_data[j] = exp(input_output_data);
// 		sum += output_data[j];
// 	}
// 	for(int j = 0; j < classes; j++){
// 		output_data[j] = output_data[j]/sum;
// 	}
// }

float NeuralNetwork::forward(const float Input[], const float Target[]){
    float error = 0;
    float sum = 0;
    float max = 0;

    float alpha = 0.1;

    // float tmpH[HiddenNodes] = {};
    // float tmpO[OutputNodes] = {};
    float Accum[OutputNodes] = {};

    /******************************************************************
    * Compute hidden layer activations
    ******************************************************************/
    for (int i = 0; i < HiddenNodes; i++) {
        float Accum = HiddenWeights[InputNodes*HiddenNodes + i];
        for (int j = 0; j < InputNodes; j++) {
            Accum += Input[j] * HiddenWeights[j*HiddenNodes + i];
        }
        Hidden[i] = 1.0 / (1.0 + exp(-Accum));

        // if(Accum > 0) {
        //     Hidden[i] = Accum;
        // }
        // else {
        //     Hidden[i] = alpha * (exp(Accum) - 1);
        //     // Hidden[i] = alpha * Accum;
        // }

        // Hidden[i] = fmax(0, Accum);
        // Hidden[i] = exp(tmpH[i] - max);
        // sum += Hidden[i];
    }
    // for(int i = 0; i < HiddenNodes; i++) {
    //     Hidden[i] /= sum;
    // }

    /******************************************************************
    * Compute output layer activations and calculate errors
    ******************************************************************/
    sum = 0;
    max = 0;
    for (int i = 0; i < OutputNodes; i++) {
        Accum[i] = OutputWeights[HiddenNodes*OutputNodes + i];
        for (int j = 0; j < HiddenNodes; j++) {
            Accum[i] += Hidden[j] * OutputWeights[j*OutputNodes + i];
            if(Accum[i] > max) max = Accum[i];
            // Accum += tmpO[i];
        }
        // Output[i] = 1.0 / (1.0 + exp(-Accum[i]));
        // Output[i] = exp(Accum - max);
        // sum += Output[i];
        // OutputDelta[i] = (Target[i] - Output[i]) * Output[i] * (1.0 - Output[i]);
        // error += 0.33333 * (Target[i] - Output[i]) * (Target[i] - Output[i]);
        // error += 0.33333 * -Target[i] * log(Output[i]);
    }

    for(int i = 0; i < OutputNodes; i++) {
        sum += exp(Accum[i] - max);
    }

    float offset = max + log(sum);
    for(int i = 0; i < OutputNodes; i++) {
        Output[i] = exp(Accum[i] - offset);
        // OutputDelta[i] = (Target[i] - Output[i]) * Output[i] * (1.0 - Output[i]);
        error += Target[i] * log(Output[i]);
        // error -= (Target[i] * log(Output[i])) + (1.0 - Target[i]) * log(1.0 - Output[i]);
        OutputDelta[i] = Target[i] - Output[i];
    }

    return -error;
}

float NeuralNetwork::backward(const float Input[], const float Target[]){
    float error = 0;
    float sum = 0;
    float max = 0;

    float alpha = 0.1;

    // float tmpH[HiddenNodes] = {};
    // float tmpO[OutputNodes] = {};
    float Accum0[HiddenNodes] = {};
    float Accum[OutputNodes] = {};


    // Forward
    /******************************************************************
    * Compute hidden layer activations
    ******************************************************************/
    for (int i = 0; i < HiddenNodes; i++) {
        Accum0[i] = HiddenWeights[InputNodes*HiddenNodes + i];
        for (int j = 0; j < InputNodes; j++) {
            Accum0[i] += Input[j] * HiddenWeights[j*HiddenNodes + i];
            // if(tmpH[i] > max) max = tmpH[i];
            // Accum += tmpH[i];
        }
        Hidden[i] = 1.0 / (1.0 + exp(-Accum0[i]));

        // if(Accum0[i] > 0) {
        //     Hidden[i] = Accum0[i];
        // }
        // else {
        //     Hidden[i] = alpha * (exp(Accum0[i]) - 1);
        //     // Hidden[i] = alpha * Accum0[i];
        // }

        // Hidden[i] = fmax(0, Accum);
        // Hidden[i] = exp(tmpH[i] - max);
        // sum += Hidden[i];
    }
    // for(int i = 0; i < HiddenNodes; i++) {
    //     Hidden[i] /= sum;
    // }

    /******************************************************************
    * Compute output layer activations and calculate errors
    ******************************************************************/
    sum = 0;
    max = 0;
    for (int i = 0; i < OutputNodes; i++) {
        Accum[i] = OutputWeights[HiddenNodes*OutputNodes + i];
        for (int j = 0; j < HiddenNodes; j++) {
            Accum[i] += Hidden[j] * OutputWeights[j*OutputNodes + i];
            if(Accum[i] > max) max = Accum[i];
            // Accum += tmpO[i];
        }
        // Output[i] = 1.0 / (1.0 + exp(-Accum[i]));
        // Output[i] = exp(Accum - max);
        // sum += Output[i];
        // OutputDelta[i] = (Target[i] - Output[i]) * Output[i] * (1.0 - Output[i]);
        // error += 0.33333 * (Target[i] - Output[i]) * (Target[i] - Output[i]);
        // error += 0.33333 * -Target[i] * log(Output[i]);
    }

    for(int i = 0; i < OutputNodes; i++) {
        sum += exp(Accum[i] - max);
    }

    float offset = max + log(sum);
    for(int i = 0; i < OutputNodes; i++) {
        Output[i] = exp(Accum[i] - offset);
        // OutputDelta[i] = (Target[i] - Output[i]) * Output[i] * (1.0 - Output[i]);
        error += Target[i] * log(Output[i]);
        // error -= (Target[i] * log(Output[i])) + (1.0 - Target[i]) * log(1.0 - Output[i]);
        OutputDelta[i] = Target[i] - Output[i];
    }
    // End forward

    // Backward
    /******************************************************************
    * Backpropagate errors to hidden layer
    ******************************************************************/
    for(int i = 0 ; i < HiddenNodes ; i++ ) {    
        float Accum = 0.0;
        for(int j = 0 ; j < OutputNodes ; j++ ) {
            Accum += OutputWeights[i*OutputNodes + j] * OutputDelta[j] ;
        }

        // if (Accum0[i] > 0) {
        //     HiddenDelta[i] = Accum * 1;
        // }
        // else {
        //     HiddenDelta[i] = Accum * (Hidden[i] + alpha);
        //     // HiddenDelta[i] = Accum * alpha;
        // }

        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]) ;
    }

    /******************************************************************
    * Update Inner-->Hidden Weights
    ******************************************************************/
    for(int i = 0 ; i < HiddenNodes ; i++ ) {   
        float change_pre = ChangeHiddenWeights[InputNodes*HiddenNodes + i];  
        ChangeHiddenWeights[InputNodes*HiddenNodes + i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes*HiddenNodes + i];
        // HiddenWeights[InputNodes*HiddenNodes + i] += ChangeHiddenWeights[InputNodes*HiddenNodes + i] ;
        ChangeHiddenWeights[InputNodes*HiddenNodes + i] += Momentum * (ChangeHiddenWeights[InputNodes*HiddenNodes + i] - change_pre);
        HiddenWeights[InputNodes*HiddenNodes + i] += ChangeHiddenWeights[InputNodes*HiddenNodes + i];
        for(int j = 0 ; j < InputNodes ; j++ ) { 
            float change_pre2 = ChangeHiddenWeights[j*HiddenNodes + i];  
            ChangeHiddenWeights[j*HiddenNodes + i] = LearningRate * Input[j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j*HiddenNodes + i];
            // HiddenWeights[j*HiddenNodes + i] += ChangeHiddenWeights[j*HiddenNodes + i] ;
            ChangeHiddenWeights[j*HiddenNodes + i] += Momentum * (ChangeHiddenWeights[j*HiddenNodes + i] - change_pre2);
            HiddenWeights[j*HiddenNodes + i] += ChangeHiddenWeights[j*HiddenNodes + i];
        }
    }

    /******************************************************************
    * Update Hidden-->Output Weights
    ******************************************************************/
    for(int i = 0 ; i < OutputNodes ; i ++ ) {    
        float change_pre = ChangeOutputWeights[HiddenNodes*OutputNodes + i];
        ChangeOutputWeights[HiddenNodes*OutputNodes + i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes*OutputNodes + i];
        // OutputWeights[HiddenNodes*OutputNodes + i] += ChangeOutputWeights[HiddenNodes*OutputNodes + i] ;
        ChangeOutputWeights[HiddenNodes*OutputNodes + i] += Momentum * (ChangeOutputWeights[HiddenNodes*OutputNodes + i] - change_pre);
        OutputWeights[HiddenNodes*OutputNodes + i] += ChangeOutputWeights[HiddenNodes*OutputNodes + i];
        for(int j = 0 ; j < HiddenNodes ; j++ ) {
            float change_pre2 = ChangeOutputWeights[j*OutputNodes + i];
            ChangeOutputWeights[j*OutputNodes + i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j*OutputNodes + i];
            // OutputWeights[j*OutputNodes + i] += ChangeOutputWeights[j*OutputNodes + i] ;
            ChangeOutputWeights[j*OutputNodes + i] += Momentum * (ChangeOutputWeights[j*OutputNodes + i] - change_pre2);
            OutputWeights[j*OutputNodes + i] += ChangeOutputWeights[j*OutputNodes + i];
        }
    }

    return -error;
}


float* NeuralNetwork::get_output(){
    return Output;
}

float* NeuralNetwork::get_HiddenWeights(){
    return HiddenWeights;
}

float* NeuralNetwork::get_OutputWeights(){
    return OutputWeights;
}

float NeuralNetwork::get_error(){
    return Error;
}