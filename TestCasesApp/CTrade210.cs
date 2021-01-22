using FileHelpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  /// <summary>
  /// 
  /// </summary>
  [Serializable]
  [DelimitedRecord(";")]
  public class CTrade210
  {
    [FieldNullValue(typeof(string), "VER.2.1")]
    public string Version { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(string), "")]
    public string Code { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(ulong), "0")]
    public ulong Login { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int Order { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(CustodyType), "Other")]
    public CustodyType Action { get; set; }

    [FieldConverter(typeof(DateConvertor))]
    [FieldNullValue(typeof(DateTime), "1970-01-01")]
    public DateTime OpenTime { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(typeof(Opstring))]
    [FieldNullValue(typeof(Opstring), "Unknown")]
    public Opstring Cmd { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double ContractSize { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int Leverage { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(string), "")]
    public string DptCry { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(string), "")]
    public string Symbol { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double OpenPrice { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double SL { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double TP { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(typeof(DateConvertor))]
    [FieldNullValue(typeof(DateTime), "1970-01-01")]
    public DateTime CloseTime { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .      
    [FieldNullValue(typeof(double), "0")]
    public double ClosePrice { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Commission { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Taxes { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Storage { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Profit { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(string), "")]
    public string BaseCry { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(string), "")]
    public string MarginCurrency { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double ConvRateOpen { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The doubleseparator is .
    [FieldNullValue(typeof(double), "0")]
    public double ConvRateClose { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(typeof(DateConvertor))]
    [FieldNullValue(typeof(DateTime), "1970-01-01")]
    public DateTime LogTime { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Bid { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Ask { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int Volume { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double MarginDivider { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldQuoted(QuoteMode.OptionalForBoth)]
    [FieldNullValue(typeof(string), "")]
    public string Company { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int MainOrder { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int MainVolume { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double TickSize { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double TickPrice { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int ExecType { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double AgentCommission { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(string), "")]
    public string Group { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int SpreadDiff { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Point { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int Digits { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int AgentAccount { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(ulong), "0")]
    public ulong GwOrder { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double MarginRate { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(typeof(DateConvertor))]
    [FieldNullValue(typeof(DateTime), "1970-01-01")]
    public DateTime Expiration { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldQuoted(QuoteMode.OptionalForBoth)]
    [FieldNullValue(typeof(string), "")]
    public string IP { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(short), "0")]
    public short GwOpenPrice { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(short), "0")]
    public short GwClosePrice { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int GwVolume { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int MarginStopout { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(ProfitCalculation), "Forex")]
    public ProfitCalculation ProfitMode { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(MARGIN_CALCULATION), "Forex")]
    public MARGIN_CALCULATION MarginMode { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double MarginInitial { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double MarginMaintenance { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldQuoted(QuoteMode.OptionalForBoth)]
    [FieldNullValue(typeof(string), "")]
    public string Description { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(Reason), "Unknown")]
    public Reason Reason { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int LargerLeg { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(string), "")]
    public string Entry { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldQuoted(QuoteMode.OptionalForBoth)]
    [FieldNullValue(typeof(string), "")]
    public string Server { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldQuoted(QuoteMode.OptionalForBoth)]
    [FieldNullValue(typeof(string), "")]
    public string Status { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(int), "0")]
    public int MarginCall { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Price { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(ConverterKind.Double, ".")] // The double separator is .
    [FieldNullValue(typeof(double), "0")]
    public double Adjustment { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(typeof(DateConvertor))]
    [FieldNullValue(typeof(DateTime), "1970-01-01")]
    public DateTime StartingTime { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldConverter(typeof(DateConvertor))]
    [FieldNullValue(typeof(DateTime), "1970-01-01")]
    public DateTime ExpirationTime { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldNullValue(typeof(double), "0")]
    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    public double DailyStorage { get; set; }
    /// <summary>
    /// 
    /// </summary>
    [FieldQuoted(QuoteMode.OptionalForBoth)]
    [FieldNullValue(typeof(string), "")]
    public string Comment { get; set; }

    [FieldQuoted(QuoteMode.OptionalForBoth)]
    [FieldNullValue(typeof(string), "")]
    [FieldOptional]
    public string Scenario { get; set; }

    [FieldConverter(ConverterKind.Double, ".")] // The Double separator is .
    [FieldNullValue(typeof(double), "0")]
    [FieldOptional]
    public double Amount { get; set; }
  }
}
