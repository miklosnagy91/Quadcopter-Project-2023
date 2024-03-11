#ifndef PID_h
#define PID_h

class PID{
  private:
    float m_P;
    float m_I;
    float m_D;
    float m_dt;
    float m_Error;
    float m_prevError;
    float m_proportionalTerm;
    float m_derivativeTerm;
    float m_integralTerm;
    float m_maxProportionalTerm;
    float m_maxIntegralTerm;
    float m_maxDerivativeTerm;
    float m_maxPIDResult;
  public:
    PID(float P, float I, float D, float dt) : m_P(P), m_I(I), m_D(D), m_dt(dt) {
      m_maxProportionalTerm = 100;
      m_maxIntegralTerm = 50;
      m_maxDerivativeTerm = 50;
      m_proportionalTerm = 0;
      m_integralTerm = 0;
      m_derivativeTerm = 0;
    }
    
    void calculate(float y, float r, float * PIDResult) {
      
      m_Error = r - y;
      m_proportionalTerm = m_P*m_Error;
      m_integralTerm = m_integralTerm + m_I*(m_Error + m_prevError)*m_dt/2;
      m_derivativeTerm = m_D*(m_Error - m_prevError)/m_dt;
      m_prevError = m_Error;

      
      //if (m_proportionalTerm > m_maxProportionalTerm) m_proportionalTerm = m_maxProportionalTerm;
      //else if (m_proportionalTerm < -m_maxProportionalTerm) m_proportionalTerm = -m_maxProportionalTerm;

      if (m_integralTerm > m_maxIntegralTerm) m_integralTerm = m_maxIntegralTerm;
      else if (m_integralTerm < -m_maxIntegralTerm) m_integralTerm = -m_maxIntegralTerm;

      if (m_derivativeTerm > m_maxDerivativeTerm) m_derivativeTerm = m_maxDerivativeTerm;
      else if (m_derivativeTerm < -m_maxDerivativeTerm) m_derivativeTerm = -m_maxDerivativeTerm;
 
      *PIDResult = m_proportionalTerm + m_integralTerm + m_derivativeTerm;
      
    }

    void print(void){
      Serial.print("Error = ");
      Serial.print(m_Error);
      Serial.print(" Proportional Term = ");
      Serial.print(m_proportionalTerm);
      Serial.print(" Integral Term = ");
      Serial.print(m_integralTerm);
      Serial.print(" Derivative Term = ");
      Serial.println(m_derivativeTerm);
    }

    void reset(void) {
      m_proportionalTerm = 0; 
      m_maxIntegralTerm = 0; 
      m_derivativeTerm = 0;
    }

};
#endif