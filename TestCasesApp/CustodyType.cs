namespace TestCasesApp
{
  /// <summary>
  /// 
  /// </summary>
  public enum CustodyType : uint
  {
    /// <summary>
    /// new position
    /// </summary>
    PA = 0,
    /// <summary>
    /// position close
    /// </summary>
    PK,
    /// <summary>
    /// position modify
    /// </summary>
    DG,
    /// <summary>
    /// pending order
    /// </summary>
    EM,
    /// <summary>
    /// cancel order
    /// </summary>
    IP,
    UC,
    OR,
    /// <summary>
    /// balance
    /// </summary>
    TI,
    TD,
    GS,
    /// <summary>
    /// open position
    /// </summary>
    AP = 10,
    UD,
    WO,
    RE,
    CR,
    AC,
    /// <summary>
    /// take profit
    /// </summary>
    TP,
    /// <summary>
    /// stop loss
    /// </summary>
    SL,
    PAUD,
    PAK,
    DO,
    Other,
    UNK = 4294967295
  }
}
