package game

var Csv *CsvConfig

func init() {
	Csv = new(CsvConfig)
	Csv.Init()
}
