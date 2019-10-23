import requests
import unittest

class TestPostClientsMethods(unittest.TestCase):

    def setUp(self):
        self.url = 'http://localhost:8888/clients'
        self.headers = {'Authorization' : '07b3bfe728954619b58f0107ad73acc1'}
        self.json = { 'clients' : [ '192.168.2.1', '192.168.2.2' ] }

    def test_post(self):
        returned = requests.post(self.url, headers=self.headers, json=self.json)
        self.assertEqual(returned.status_code, 200)

if __name__ == '__main__':
    unittest.main()