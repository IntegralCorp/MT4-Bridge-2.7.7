using FileHelpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  [DelimitedRecord(";")]
  public class CRawReport
  {
    [FieldNullValue(typeof(int), "0")]
    public int Deal{ get; set; }

    [FieldNullValue(typeof(int), "0")]
    public int Login{ get; set; }

    public string OpenTime{ get; set; }

    public string Type{ get; set; }

    public string Symbol{ get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Volume{ get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    [FieldNullValue(typeof(double), "0")]
    public double OpenPrice { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    [FieldNullValue(typeof(double), "0")]
    public double StopLossL { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    [FieldNullValue(typeof(double), "0")]
    public double TakeProfit { get; set; }

    public string CloseTime{ get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    [FieldNullValue(typeof(double), "0")]
    public double ClosePrice { get; set; }

    public string Reason{ get; set; }
    public string GatewayOrder{ get; set; }
    public string GatewayVolume{ get; set; }
    public string OpenPriceDelta{ get; set; }
    public string ClosePriceDelta{ get; set; }
    public string Agent{ get; set; }
    public string Commission{ get; set; }
    public string Taxes{ get; set; }
    public string Swap{ get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The decimal separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Profit{ get; set; }

    public string Points{ get; set; }
    public string Comment{ get; set; }

    [FieldOptional]
    [FieldNullValue(typeof(string), "")]
    public string Dummy { get; set; }
  }
}
