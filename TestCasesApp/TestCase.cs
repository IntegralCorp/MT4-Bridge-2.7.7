using FileHelpers;
using System;

namespace TestCasesApp
{
  [Serializable]
  [DelimitedRecord(",")]
  public class TestCase
  {
    [FieldNullValue(typeof(string), "OrderSend")]
    public string Command { get; set; }

    [FieldNullValue(typeof(string), "TC1")]
    public string CaseName { get; set; }

    public string Symbol { get; set; }

    [FieldNullValue(typeof(string), "OP_BUY")]
    public string Type { get; set; }

    [FieldNullValue(typeof(string), "1.0")]
    public string Lots { get; set; }

    [FieldNullValue(typeof(string), "0")]
    public string Price { get; set; }

    [FieldNullValue(typeof(string), "0")]
    public string Slippage { get; set; }

    [FieldNullValue(typeof(string), "0")]
    public string TP { get; set; }

    [FieldNullValue(typeof(string), "0")]
    public string SL { get; set; }
  }
}
