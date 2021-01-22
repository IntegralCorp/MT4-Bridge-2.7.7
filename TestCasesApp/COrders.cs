using FileHelpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  [DelimitedRecord(",")]
  public class COrders
  {
    public enum Op
    {
      OP_BUY,
      OP_SELL,
      OP_BUYLIMIT,
      OP_SELLLIMIT,
      OP_BUYSTOP,
      OP_SELLSTOP
    }

    [FieldNullValue(typeof(string), "")]
    public string Time { get; set; }

    [FieldNullValue(typeof(string), "")]
    public string Command { get; set; }

    [FieldNullValue(typeof(string), "")]
    public string Scenario { get; set; }

    [FieldNullValue(typeof(int), "0")]
    [FieldConverter(ConverterKind.Int32)]
    public int Order { get; set; }

    [FieldNullValue(typeof(string), "")]
    public string Symbol { get; set; }

    [FieldNullValue(typeof(Op), "OP_BUY")]
    public Op Operation { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    public double Lots { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    public double Price { get; set; }

    [FieldNullValue(typeof(int), "0")]
    public int Slippage { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    public double StopLoss { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    public double TakeProfit { get; set; }

    [FieldNullValue(typeof(string), "")]
    public string Comment { get; set; }
  }
}
