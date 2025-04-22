#ifndef PID_h
#define PID_h

// ============================================
// PID Controller Class
// ============================================
class PID {
private:
  // PID gains
  float m_P;
  float m_I;
  float m_D;

  // Time interval between updates
  float m_dt;

  // Internal state variables
  float m_Error;
  float m_prevError;
  float m_proportionalTerm;
  float m_integralTerm;
  float m_derivativeTerm;

  // Maximum limits for each PID term
  float m_maxProportionalTerm;
  float m_maxIntegralTerm;
  float m_maxDerivativeTerm;

public:
  // Constructor with PID gains and update period
  PID(float P, float I, float D, float dt)
    : m_P(P), m_I(I), m_D(D), m_dt(dt),
      m_Error(0), m_prevError(0),
      m_proportionalTerm(0), m_integralTerm(0), m_derivativeTerm(0)
  {
    // Default term limits (can be adjusted as needed)
    m_maxProportionalTerm = 100;
    m_maxIntegralTerm = 50;
    m_maxDerivativeTerm = 50;
  }

  // Main PID calculation
  void calculate(float y, float r, float* PIDResult) {
    // y: current value (feedback), r: desired value (setpoint)

    m_Error = r - y;

    // Proportional term
    m_proportionalTerm = m_P * m_Error;

    // Integral term using trapezoidal rule
    m_integralTerm += m_I * (m_Error + m_prevError) * m_dt / 2;

    // Derivative term
    m_derivativeTerm = m_D * (m_Error - m_prevError) / m_dt;

    // Save error for next cycle
    m_prevError = m_Error;

    // Clamp integral term to avoid windup
    if (m_integralTerm > m_maxIntegralTerm) m_integralTerm = m_maxIntegralTerm;
    else if (m_integralTerm < -m_maxIntegralTerm) m_integralTerm = -m_maxIntegralTerm;

    // Clamp derivative term
    if (m_derivativeTerm > m_maxDerivativeTerm) m_derivativeTerm = m_maxDerivativeTerm;
    else if (m_derivativeTerm < -m_maxDerivativeTerm) m_derivativeTerm = -m_maxDerivativeTerm;

    // Combine all terms into final output
    *PIDResult = m_proportionalTerm + m_integralTerm + m_derivativeTerm;
  }

  // Debug print of internal PID terms
  void print(void) {
    Serial.print("Error = ");
    Serial.print(m_Error);
    Serial.print(" | P = ");
    Serial.print(m_proportionalTerm);
    Serial.print(" | I = ");
    Serial.print(m_integralTerm);
    Serial.print(" | D = ");
    Serial.println(m_derivativeTerm);
  }

  // Reset all PID terms (used when stopping motors)
  void reset(void) {
    m_proportionalTerm = 0;
    m_integralTerm = 0;
    m_derivativeTerm = 0;
    m_prevError = 0;
  }
};

#endif
